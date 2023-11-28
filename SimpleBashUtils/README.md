# Simple Bash Utils
Development of a simple CI/CD for the Bash text utilities: cat, grep.

## cat Usage
Cat is one of the most frequently used commands on Unix-like operating systems. It has three related functions with regard to text files: displaying them, combining copies of them and creating new ones.

`cat [OPTION] [FILE]...`

### cat Options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -b (GNU: --number-nonblank) | numbers only non-empty lines |
| 2 | -e implies -v (GNU only: -E the same, but without implying -v) | but also display end-of-line characters as $  |
| 3 | -n (GNU: --number) | number all output lines |
| 4 | -s (GNU: --squeeze-blank) | squeeze multiple adjacent blank lines |
| 5 | -t implies -v (GNU: -T the same, but without implying -v) | but also display tabs as ^I  |


## grep Usage
`grep [options] template [file_name]`

### grep Options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -e | pattern |
| 2 | -i | Ignore uppercase vs. lowercase.  |
| 3 | -v | Invert match. |
| 4 | -c | Output count of matching lines only. |
| 5 | -l | Output matching files only.  |
| 6 | -n | Precede each matching line with a line number. |
| 7 | -h | Output matching lines without preceding them by file names. |
| 8 | -s | Suppress error messages about nonexistent or unreadable files. |
| 9 | -f file | Take regexes from a file. |
| 10 | -o | Output the matched parts of a matching line. |

## How to use
Clone repository!

Enter util's directory and compile executable files for both utils with command:

`make`

Then you can use it like this `./s21_cat simple.txt` or `./s21_grep regex simple.txt` and enjoying the result. 

Аnd also run tests for both utils (tests only for mac-os, other os can ruin some tests, because of different versions of original utils):

`make test`

Also developed CI/CD pipeline for this project and added notifications of successful/unsuccessful pipeline execution via bot named "lostwill DO6 CI/CD" in Telegram
