#!/bin/sh

# FIXME should the LC_ALL=C option be used or some national character sets
#   are used for e.g. comments?
# FIXME if some error occured, we do not clean the created files; should we?

LC_ALL='C'
IFS=' 	
'
NAME="$(basename "$0")"

print_help()
{
  cat >&2 <<__HELP__
NAME
  $NAME - convert src format to XML

SYNOPSIS
  $NAME src_file dst_file

EXIT STATUS
  0  src_file was already in the appropriate format (no conversion made)
  1  conversion was successful, dst_file was created
  >1 something went wrong, see stderr (dst_file may or may not be available)
__HELP__
}

[ "$#" -eq 2 ] || { print_help; exit 2; }

export F_IN="$1"
F_OUT="$2"

exit_fn()
{
  exit "$1"
}

if file "$F_IN" | cut -c $(echo "$F_IN" | wc -m)- | grep 'XML' > /dev/null; then
  echo "It is an XML document yet." >&2
  exit_fn 0
else
  cat "$F_IN" | F_IN="$(basename "$F_IN")" awk '
  BEGIN {
    NMSPC = "disam:"  # namespace
    delete footers[0]  # reserve var name for array
    footers_cnt = -1

    f_in = ENVIRON["F_IN"]
    sub(/[.]+$/, "", f_in)  # remove trailing dots
    sub(/[.][^.]+$/, "", f_in)  # remove once the extension
    # for case filename was only from dots
    if (! f_in) { f_in = ENVIRON["F_IN"] }

    print "<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
    print "<" NMSPC "configuration id=\"" f_in "\" version=\"1.0\"" \
          " xmlns:disam=\"http://www.disam.cz/Xmlns/Scada/Configuration\">"
#      RS="[[:space:]]"
  }

  function print_footers(   x) {
    while (footers_cnt >= 0) {
      print footers[footers_cnt--]
    }

    printf("\n")

    for (x in footers) { delete footers[x] }
  }

  # changes global arrays notes[] and items[]
  function handle_item(line,   newrec, x, ret) {
    for (x in notes) { delete notes[x] }
    for (x in items) { delete items[x] }
    x = 0

    # separate notes from double quotes (and substite them for integer keys
    # into hash table in the form "123" (double quotes inclusive) used as
    # an index into notes[] array)
    while (match(line, /"([^"\\]*(\\.[^"\\]*)*)"/)) {
      notes[x] = substr(line, RSTART +1, RLENGTH -2)
      newrec = newrec substr(line, 1, RSTART -1) "\"" x "\""
      line = substr(line, RSTART + RLENGTH)
      x++
    }

    newrec = newrec line
    sub(/[,;]+$/, "", newrec)
    ret = split(newrec, items, /[,;]+/)

    # remove opening and trailing spaces
    for (x in items) {
      sub(/^[[:space:]]+/, "", items[x])
      sub(/[[:space:]]+$/, "", items[x])
    }

    return ret
  }

  {
    # convert CRLF to LF
    sub(/\r$/, "", $0)

    # silently ignore empty lines
    if ($0 ~ /^[[:space:]]*$/) {
      next
    }

    else if (sub(/^[[:space:]]*:/, "", $0)) {
    # new section {{{
      # FIXME
      #if (eq_sign_found_in_data) {
      #  print "/>"
      #  eq_sign_found_in_data=""
      #}
      eq_sign_found_in_data=""

      print_footers()

      # normalize FIXME what about other chars like &"''<>
      gsub(/[[:space:]_]+/, "-", $0)

      section = tolower($0)
      match(section, /^[^A-Za-z]*[A-Za-z]+/)
      sec_prefix = substr(section, RSTART, RLENGTH)

      for (x in format) { delete format[x] }
      for (x in format_fin) { delete format_fin[x] }
      ii = 0;
      backslashfound = "true"
      formatprocessed = ""
      depth = 0

      allow_cmt = "true"  # next line shall be a coment
    }
    # }}}

    else if (sub(/^[[:space:]]*#[[:space:]]*---+[[:space:]]*/, "", $0)) {
    # human comment {{{
      print "line " NR ": " $0 | "cat 1>&2"
    }
    # }}}

    else if (sub(/^[[:space:]]*#[[:space:]]*/, "", $0)) {
    # comment {{{
      if (allow_cmt) {
        if (backslashfound)
          format[ii] = format[ii] $0
        else
          format[++ii] = $0

        if (sub(/[[:space:]]*\\$/, "", format[ii])) {
          backslashfound = "true"
        }
        else {
          backslashfound = ""
        }
      }
      # unnecessary comment found => warn
      else {
        print "line " NR ": unsupported comment found, " \
              "use the \"description\" field instead" | "cat 1>&2"
      }
    }
    # }}}

    # data
    else {
      if (1 in format) {
      # there was a special multiline comment with {} {{{
        # format
        if (!formatprocessed) {
          x = 0
          maxdepth = 0  # maximum nesting depth
          emerging = ""  # ensure only one "recursion" (assume each
                         # item on the same level has the same signature)

          for (i = 0; i <= ii; ++i) {
            if (format[i] ~ /[[:space:]]*[{][[:space:]]*/) {
              if (!emerging) maxdepth++
            }
            else if (format[i] ~ /[[:space:]]*[}][[:space:]]*/) {
              emerging = "true"
            }
            # assume only first line is signature
            else if (!format_fin[maxdepth, -1] && !emerging) {
              for (j in res) { delete res[j] }  # not needed, but for sure

              sub(/[][,;]+$/, "", format[i])
              format_fin[maxdepth, -1] = split(format[i], res, /[][,;]+/)

              for (j = 1; j <= format_fin[maxdepth, -1]; j++) {
                # remove opening and trailing spaces first
                sub(/^[[:space:]]+/, "", res[j])
                sub(/[[:space:]]+$/, "", res[j])

                # normalize
                gsub(/[[:space:]_]+/, "-", res[j])

                format_fin[maxdepth, j] = res[j]
              }
            }
          }

          print "<" NMSPC section ">"
          footers[++footers_cnt] = "</" NMSPC section ">"

          formatprocessed = "true"
        }

        if ($0 ~ /[{]/) {
          depth++
        }
        else if ($0 ~ /[}]/) {
          # we are emerging by one level
          print footers[footers_cnt--]
          depth--
        }
        # some item
        else {
          # creates global array notes[] and items[]
          item_cnt = handle_item($0)

          # type
          if (depth == 0) {
            print "<" NMSPC sec_prefix "-type id=\"" items[1] "\">"
            footers[++footers_cnt] = "</" NMSPC sec_prefix "-type>"
          }
          # subtype
          else if (depth < maxdepth) {
            print "<" NMSPC sec_prefix "-subtype id=\"" items[1] "\">"
            footers[++footers_cnt] = "</" NMSPC sec_prefix "-subtype>"
          }
          # final item itself
          else {
            printf("<%s%s-item", NMSPC, sec_prefix)

            for (i = 1; i <= format_fin[depth, -1]; i++) {
              if (items[i] ~ /^"[0-9]+"$/) {
                printf(" %s=\"%s\"",
                        format_fin[depth, i],
                        notes[substr(items[i], 2, length(items[i]) -2)])
              }
              else {
                printf(" %s=\"%s\"",
                        format_fin[depth, i],
                        items[i])
              }
            }

            print "/>"
          }
        }
      }
      # }}}

      else {
        if ($0 ~ /^[[:space:]]*[^[:space:]="]+=.*/) {
        # special data in the form key=value; {{{
          if (! formatprocessed) {
            print "<" NMSPC section ">"
            footers[++footers_cnt] = "</" NMSPC section ">"

            formatprocessed = "true"
          }

          if (! eq_sign_found_in_data) {
            printf("<%s%s-item", NMSPC, sec_prefix)
            footers[++footers_cnt] = "/>"

            eq_sign_found_in_data = "true"
          }

          split($0, key_value_pair, /[[:space:]]*=[[:space:]]*/)

          # normalize key
          gsub(/[[:space:]_]+/, "-", key_value_pair[1])

          for (x in key_value_pair) {
            # remove opening and trailing spaces and (semi)colons
            sub(/^[[:space:]]+/, "", key_value_pair[x])
            sub(/[[:space:],;]+$/, "", key_value_pair[x])
          }

          # avoid two consecutive double quotes in XML
          sub(/^"+/, "", key_value_pair[2])
          sub(/"+$/, "", key_value_pair[2])

          printf(" %s=\"%s\"", key_value_pair[1], key_value_pair[2])
        }
        # }}}

        else {
        # there was a one-line comment {{{
          if (! formatprocessed) {
            sub(/[][,;]+$/, "", format[0])
            format_fin[-1] = split(format[0], format_fin, /[][,;]+/)

            # remove constraints and if found, write it to stderr
            for (x = 1; x <= format_fin[-1]; x++) {
              # remove opening and trailing spaces first
              sub(/^[[:space:]]+/, "", format_fin[x])
              sub(/[[:space:]]+$/, "", format_fin[x])

              if (match(format_fin[x], /[[:space:]]*\([^)]*\)$/)) {
                print "line " NR ": cfgview.xml constraint `" \
                      substr(format_fin[x], RSTART, RLENGTH) \
                      "'"'"' found on one of previous lines" | "cat 1>&2"
                format_fin[x] = substr(format_fin[x], 1, RSTART -1)
              }

              # normalize
              gsub(/[[:space:]_]+/, "-", format_fin[x])
            }

            print "<" NMSPC section ">"
            footers[++footers_cnt] = "</" NMSPC section ">"

            formatprocessed = "true"
          }

          printf("<%s%s-item", NMSPC, sec_prefix)

          #FIXME !!!!!!!!!!!!!!!!!!!!! FIXME !!!!!!!!!!!!
          #assert(! format_fin.empty()) # proste, ze se tam vubec nejaky
                                       # formatovaci radek objevil

          # creates global array notes[] and items[]
          item_cnt = handle_item($0)

          for (i = 1; i <= format_fin[-1]; i++) {
            if (items[i] ~ /^"[0-9]+"$/) {
              printf(" %s=\"%s\"",
                      format_fin[i],
                      notes[substr(items[i], 2, length(items[i]) -2)])
            }
            else {
              printf(" %s=\"%s\"",
                      format_fin[i],
                      items[i])
            }
          }

          print "/>"
        }
        # }}}
      }

      # no comment on next line expected
      allow_cmt = ""
    }
  }

  END {
    #FIXME
    #if (eq_sign_found_in_data) { print "/>" }

    print_footers()
    print "</" NMSPC "configuration>"
  }
  ' > "$F_OUT" || exit_fn 2

  exit_fn 1
fi

# vim: set ft=sh:
