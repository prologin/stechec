eval '(exit $?0)' && eval 'exec perl -x -S $0 ${1+"$@"}' &&
eval 'exec perl -x -S  $0 $argv:q'
if 0;
#!/usr/bin/perl -w
#!/opt/local/bin/perl -w
#!/usr/local/bin/perl -w

#================= TO DO ================
#
# 1.  Check file of bug reports and requests


# The above code allows this script to be run under UNIX/LINUX without
# the need to adjust the path to the perl program in a "shebang" line.
# (The location of perl changes between different installations, and
# may even be different when several computers running different
# flavors of UNIX/LINUX share a copy of latex or other scripts.)  The
# script is started under the default command interpreter sh, and the
# evals in the first two lines restart the script under perl, and work
# under various flavors of sh.  The -x switch tells perl to start the
# script at the first #! line containing "perl".  The "if 0;" on the
# 3rd line converts the first two lines into a valid perl statement
# that does nothing.

# To do:
#   Rationalize again handling of include files.
#     Perhaps use kpsewhich to do searches.
#        (How do I avoid getting slowed down too much?)
#     Better parsing of log file for includes.
#   Do I handle the recursive dependence of bbl and aux files sensibly.
#     Perhaps some appropriate touching of the .aux and .bbl files would help?
#   Document the assumptions at each stage of processing algorithm.
#   Option to restart previewer automatically, if it dies under -pvc
#   Test for already running previewer gets wrong answer if another
#     process has the viewed file in its command line

$version_num = '3.07a';
$version_details = 'latexmk, John Collins, 2 June 2004';

use File::Copy;
use File::Basename;
use FileHandle;
use File::Find;

## Copyright John Collins 1998-2004
##           (username collins at node phys.psu.edu)
##      (and thanks to David Coppit (username david at node coppit.org)
##           for suggestions)
## Copyright Evan McLean
##         (modifications up to version 2)
## Copyright 1992 by David J. Musliner and The University of Michigan.
##         (original version)
##
##    This program is free software; you can redistribute it and/or modify
##    it under the terms of the GNU General Public License as published by
##    the Free Software Foundation; either version 2 of the License, or
##    (at your option) any later version.
##
##    This program is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##    GNU General Public License for more details.
##
##    You should have received a copy of the GNU General Public License
##    along with this program; if not, write to the Free Software
##    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307


##
##
##   For Win95 et al.:  Remaining items on list.
##     1.  See specific flags
##     6.  Need to check out all calls to system
##     8.  Now check that I do the bibtex and index stuff correctly.
##     9.  Convert routines to have arguments.
##    12.  Need to have default filetypes for graphics includes.
##         To make that rational, the find_file routine, etc must be modified,
##         otherwise there will be much duplication of code.
##
##
##   NEW FEATURES, since v. 2.0:
##     1.  Correct algorithm for deciding how many times to run latex:
##         based on whether the aux file changes between runs
##     2.  Continuous preview works, and can be of ps file or dvi file
##     3.  pdf creation by pdflatex possible
##     4.  Defaults for commands are OS dependent.
##     5.  Parsing of log file instead of source file is used to
##         obtain dependencies, by default.
##     6.  Meta-commands in source file
##
##           %#{}raw
##           %#{}begin.raw
##              Start section of raw LaTeX: treat as %#{latex}begin.ignore.
##
##           %#{}end.raw
##              End section of raw LaTeX: treat as %#{latex}end.ignore.
##
##           %#{latexmk}ignore
##           %#{latexmk}begin.ignore
##              Start section that is to be ignored in scanning dependencies.
##           %#{latexmk}end.ignore
##              End section that is to be ignored in scanning dependencies.
##
##           %#{latexmk}include.file FILE
##           %#{latexmk}input.file FILE
##              Add FILE to list of include files.  (FILE is separated from
##              the keyword "include.file" or "input.file" by white space,
##              and is terminated by end-of-line or whitespace.)
##
##
##
##   Modified
##
##
##       2 Jun 2004, John Collins
##         Correct bug in parsing of log files from pdflatex
##      17 May 2004, John Collins
##      16 May 2004, John Collins
##         Correct bug in using $print_type eq 'pdf'
##         Command line switches for print_type
##      14 May 2004, John Collins
##      11 May 2004, John Collins
##         Correct parse_log bug
##       3 May 2004, John Collins
##         Correct bug
##       2 May 2004, John Collins
##         Redo handling of errors in -pvc mode: $force_mode off.
##         Deal with dying in caller of &make_files (etc)
##       1 May 2004, John Collins
##         Go mode applies to custom dependencies
##         Rerun custom dependency and latex when previously non-found
##           files are found.
##      30 Apr 2004, John Collins
##         Fix bug in make_dependents
##         Add messages about up-to-date files when doing nothing
##         Silence more informational messages in silent mode.
##         In list of dependent files generated from .log file
##            excluded those whose extenstions are in @generated_exts.
##            This removes generated files from the dependency list.
##      29 Apr 2004, John Collins
##      28 Apr 2004, John Collins
##         Rationalize default file-list to be in @default_files, etc
##         Change behavior about lack of command line files:
##             Files specified on command-line but none found: error
##             No files specified on command-line:
##                Try @default_files (with wildcards)
##                If still none: error
##         Also apply sort and uniq to globbed list, to eliminate duplicates
##      27 Apr 2004, John Collins
##         Correct parsing of filenames in log file and in aux file
##      26 Apr 2004, John Collins
##         Correct calls to open, to be OK under perl 5.005 (on SUN)
##      25 Apr 2004, John Collins
##         Cleanup of comments, some error messages, etc
##         Change terminology 'include' to 'input' where appropriate
##         Switches to turn off modes: -F-, -g-, -i-, -I-, -pv-, -pvc-
##         -pv and -pvc override each other.  (Easier than making them
##            incompatible, and particularly necessary when one is
##            specified in a startup file.)
##         Die if incorrect option specified on command line.
##         Turn off bibtex mode if no .bib files specified in .aux
##            file. That would be the situation if a .bbl file was
##            input by an input command, so that the user is not
##            expecting to use bibtex.
##         Add a condition for rerun to include a change in the list
##            of input files.
##         In force_mode, don't die when files are not found; they may
##            be in a search path where I don't look.  (But I have only
##            done this for the case that I parse the log file.)
##         When files for viewing, converting (dvi to ps, etc), etc are
##            not found, give a warning rather than trying to continue.
##         Revise parsing in .log file to allow filenames with spaces.
##         Use arrays for all lists of filenames.
##       5 Apr 2004, John Collins
##         If latexmk exits because of an error with latex, bibtex
##            or makeindex, then give a non-zero exit code.
##       4 Apr 2004, John Collins
##         All options can be introduced by -- or -
##         Attempt to treat cygwin correctly
##         Complete the list of commands printed with -commands
##         Correct commands for postscript mode written to .dep file
##       3 Apr 2004, John Collins
##         V. 3.07 started
##       2 Mar 2004, John Collins
##         Obfuscate e-mail addresses so that they aren't picked up by
##            worms and spammers
##      17 Jun 2003, John Collins
##         For pdf viewing set $viewer_update_method to $pdf_update_method
##            Previously it was set wrongly to $ps_update_method
##      10 Dec 2002, John Collins
##         Recursive search of BIBINPUTS and TEXINPUTS
##         Thanks to Nevin Kapur
##       5 Nov 2002, John Collins
##         Reorganize handling of filename specification, so that
##         filenames can be specified in an rc file. E.g.,
##              @file_list = ("paper");
##      26 Oct 2002, John Collins
##         In -pvc mode keep going (i.e., -f switch), but have and -f-
##            option to turn it off.
##      25 Oct 2002, John Collins
##         Correct bug in splitting of command line
##      24 Oct 2002, John Collins
##         pdfdvi option
##         For -pvc take PID of viewer from fork.
##      22 Oct 2002, John Collins
##         Change pscmd on linux
##      12--15 Oct 2002, John Collins
##         Improve error messages.
##         Add "_switch" to names of configuration variables giving switches
##           (to avoid confusion with variables specifying commands).
##           E.g., latex_silent_switch
##         -P pdf switch on dvips, to ensure correct type 1 font generation
##           (otherwise pdf files have type 3 fonts, and look bad on screen).
##         Correct running of ps filter.
##             It now only runs when ps file is updated.
##         Some clean-ups of code.
##         Allow preview of multiple files
##         Option to generate pdf file by ps2pdf (switch -pdfps)
##         Prevent detached previewer from responding to ctrl/C given to
##             parent latemk process.
##         Under MS Windows, wildcards allowed in file names.
##      14 Apr 2002, John Collins
##         Ver. 3.05
##      24 Nov 2001, John Collins
##         Ver. 3.04.  Released on CTAN
##      11 Jun 2001, John Collins
##         Ver. 3.00.  Misc cleanup.
##
##   1998-2001, John Collins.  Many improvements and fixes.
##
##   Modified by Evan McLean (no longer available for support)
##   Original script (RCS version 2.3) called "go" written by David J. Musliner
##
## 2.0 - Final release, no enhancements.  LatexMk is no longer supported
##       by the author.
## 1.9 - Fixed bug that was introduced in 1.8 with path name fix.
##     - Fixed buglet in man page.
## 1.8 - Add not about announcement mailling list above.
##     - Added texput.dvi and texput.aux to files deleted with -c and/or
##       the -C options.
##     - Added landscape mode (-l option and a bunch of RC variables).
##     - Added sensing of "\epsfig{file=...}" forms in dependency generation.
##     - Fixed path names when specified tex file is not in the current
##       directory.
##     - Fixed combined use of -pvc and -s options.
##     - Fixed a bunch of speling errors in the source. :-)
##     - Fixed bugs in xdvi patches in contrib directory.
## 1.7 - Fixed -pvc continuous viewing to reattach to pre-existing
##       process correctly.
##     - Added $pscmd to allow changing process grepping for different
##       systems.
## 1.6 - Fixed buglet in help message
##     - Fixed bugs in detection of input and include files.
## 1.5 - Removed test message I accidentally left in version 1.4
##     - Made dvips use -o option instead of stdout redirection as some
##       people had problems with dvips not going to stdout by default.
##     - Fixed bug in input and include file detection
##     - Fixed dependency resolution process so it detects new .toc file
##       and makeindex files properly.
##     - Added dvi and postscript filtering options -dF and -pF.
##     - Added -v version commmand.
## 1.4 - Fixed bug in -pvc option.
##     - Made "-F" option include non-existant file in the dependency list.
##       (RC variable: $force_include_mode)
##     - Added .lot and .lof files to clean up list of extensions.
##     - Added file "texput.log" to list of files to clean for -c.
##     - LatexMk now handles file names in a similar fashion to latex.
##       The ".tex" extension is no longer enforced.
##     - Added $texfile_search RC variable to look for default files.
##     - Fixed \input and \include so they add ".tex" extension if necessary.
##     - Allow intermixing of file names and options.
##     - Added "-d" and banner options (-bm, -bs, and -bi).
##       (RC variables: $banner, $banner_message, $banner_scale,
##       $banner_intensity, $tmpdir)
##     - Fixed "-r" option to detect an command line syntax errors better.
## 1.3 - Added "-F" option, patch supplied by Patrick van der Smagt.
## 1.2 - Added "-C" option.
##     - Added $clean_ext and $clean_full_ext variables for RC files.
##     - Added custom dependency generation capabilities.
##     - Added command line and variable to specify custom RC file.
##     - Added reading of rc file in current directly.
## 1.1 - Fixed bug where Dependency file generation header is printed
##       rependatively.
##     - Fixed bug where TEXINPUTS path is searched for file that was
##       specified with absolute an pathname.
## 1.0 - Ripped from script by David J. Musliner (RCS version 2.3) called "go"
##     - Fixed a couple of file naming bugs
##        e.g. when calling latex, left the ".tex" extension off the end
##             of the file name which could do some interesting things
##             with some file names.
##     - Redirected output of dvips.  My version of dvips was a filter.
##     - Cleaned up the rc file mumbo jumbo and created a dependency file
##       instead.  Include dependencies are always searched for if a
##       dependency file doesn't exist.  The -i option regenerates the
##       dependency file.
##       Getting rid of the rc file stuff also gave the advantage of
##       not being restricted to one tex file per directory.
##     - Can specify multiple files on the command line or no files
##       on the command line.
##     - Removed lpr options stuff.  I would guess that generally,
##       you always use the same options in which case they can
##       be set up from an rc file with the $lpr variable.
##     - Removed the dviselect stuff.  If I ever get time (or money :-) )
##       I might put it back in if I find myself needing it or people
##       express interest in it.
##     - Made it possible to view dvi or postscript file automatically
##       depending on if -ps option selected.
##     - Made specification of dvi file viewer seperate for -pv and -pvc
##       options.
##-----------------------------------------------------------------------


#Line length in log file that indicates wrapping.
# This number EXCLUDES line-end characters, and is one-based
$log_wrap = 79;

#########################################################################
## Default document processing programs, and related settings,
## These are mostly the same on all systems.
## Most of these variables represents the external command needed to
## perform a certain action.  Some represent switches.

## Commands to invoke latex, pdflatex
$latex  = 'latex';
$pdflatex = 'pdflatex';
## Switch(es) to make them silent:
$latex_silent_switch  = '-interaction=batchmode';
$pdflatex_silent_switch  = '-interaction=batchmode';

## Command to invoke bibtex
$bibtex  = 'bibtex';
# Switch(es) to make bibtex silent:
$bibtex_silent_switch  = '-terse';

## Command to invoke makeindex
$makeindex  = 'makeindex';

## Command to convert dvi file to pdf file directly:
$dvipdf  = 'dvipdf';

## Command to convert dvi file to ps file:
$dvips  = 'dvips';
## Command to convert dvi file to ps file in landscape format:
$dvips_landscape = 'dvips -tlandscape';
# Switch(es) to get dvips to make ps file suitable for conversion to good pdf:
#    (If this is not used, ps file and hence pdf file contains bitmap fonts
#       (type 3), which look horrible under acroread.  An appropriate switch
#       ensures type 1 fonts are generated.  You can put this switch in the
#       dvips command if you prefer.)
$dvips_pdf_switch = '-P pdf';
# Switch(es) to make dvips silent:
$dvips_silent_switch  = '-q';

## Command to convert ps file to pdf file:
$ps2pdf = 'ps2pdf';

##Printing:
$print_type = 'ps';     # When printing, print the postscript file.
                        # Possible values: 'dvi', 'ps', 'pdf', 'none'

## Which treatment of default extensions and filenames with
##   multiple extensions is used, for given filename on
##   tex/latex's command line?  See sub find_basename for the
##   possibilities.
## Current tex's treat extensions like UNIX teTeX:
$extension_treatment = 'unix';

#########################################################################

# System-dependent overrides:
if ( $^O eq "MSWin32" ) {
# Pure MSWindows configuration
    ## Configuration parameters:
    $tmpdir = "$ENV{TEMP}";

    ## List of possibilities for the system-wide initialization file.
    ## The first one found (if any) is used.
    @rc_system_files = ( 'C:/latexmk/LatexMk' );

    $search_path_separator = ';';  # Separator of elements in search_path

    # For both fptex and miktex, the following makes error messages explicit:
    $latex_silent_switch  = '-interaction=batchmode -c-style-errors';
    $pdflatex_silent_switch  = '-interaction=batchmode -c-style-errors';

    # For a pdf-file, "start x.pdf" starts the pdf viewer associated with
    #   pdf files, so no program name is needed:
    $pdf_previewer = 'start';
    $ps_previewer  = 'start';
    $ps_previewer_landscape  = "$ps_previewer";
    $dvi_previewer  = 'start';
    $dvi_previewer_landscape = "$dvi_previewer";
    # Viewer update methods:
    #    0 => auto update: viewer watches file (e.g., gv)
    #    1 => manual update: user must do something: e.g., click on window.
    #         (e.g., ghostview, MSWIN previewers, acroread under UNIX)
    #    2 => send SIGUSR1 signal (xdvi)
    #    3 => viewer can't update, because it locks the file and the file
    #         cannot be updated.  (acroread under MSWIN)
    $dvi_update_method = 1;
    $ps_update_method = 1;
    $pdf_update_method = 3; # acroread locks the pdf file
    $lpr  = 'NONE lpr';        # Use NONE as flag that I am not implementing this
    $lpr_dvi  = 'NONE lpr_dvi';# Use NONE as flag that I am not implementing this
    $lpr_pdf  = 'NONE lpr_pdf';# Use NONE as flag that I am not implementing this
    # The $pscmd below holds a command to list running processes.  It
    # is used to find the process ID of the viewer looking at the
    # current output file.  The output of the command must include the
    # process number and the command line of the processes, since the
    # relevant process is identified by the name of file to be viewed.
    # Its use is not essential.
    $pscmd = 'NONE pscmd';  # Use NONE as flag that I am not implementing this
    $pid_position = -1;     # offset of PID in output of pscmd.
                            # Negative means I cannot use ps
}
elsif ( $^O eq "cygwin" ) {
    # The problem is a mixed MSWin32 and UNIX environment.
    # Perl decides the OS is cygwin in two situations:
    # 1. When latexmk is run from a cygwin shell under a cygwin
    #    environment.  Perl behaves in a UNIX way.  This is OK, since
    #    the user is presumably expecting UNIXy behavior.
    # 2. When CYGWIN exectuables are in the path, but latexmk is run
    #    from a native NT shell.  Presumably the user is expecting NT
    #    behavior. But perl behaves more UNIXy.  This causes some
    #    clashes.
    # The issues to handle are:
    # 1.  Perl sees both MSWin32 and cygwin filenames.  This is
    #     normally only an advantage.
    # 2.  Perl uses a UNIX shell in the system command
    #     This is a nasty problem: under native NT, there is a
    #     start command that knows about NT file associations, so that
    #     we can do, e.g., (under native NT) system("start file.pdf");
    #     But this won't work when perl has decided the OS is cygwin,
    #     even if it is invoked from a native NT command line.  An
    #     NT command processor must be used to deal with this.
    # 3.  External executables can be native NT (which only know
    #     NT-style file names) or cygwin executables (which normally
    #     know both cygwin UNIX-style file names and NT file names,
    #     but not always; some do not know about drive names, for
    #     example).
    #     Cygwin executables for tex and latex may only know cygwin
    #     filenames.
    # 4.  The BIBINPUTS and TEXINPUTS environment variables may be
    #     UNIX-style or MSWin-style depending on whether native NT or
    #     cygwin executables are used.  They are therefore parsed
    #     differently.  Here is the clash:
    #        a. If a user is running under an NT shell, is using a
    #           native NT installation of tex (e.g., fptex or miktex),
    #           but has the cygwin executables in the path, then perl
    #           detects the OS as cygwin, but the user needs NT
    #           behavior from latexmk.
    #        b. If a user is running under an UNIX shell in a cygwin
    #           environment, and is using the cygwin installation of
    #           tex, then perl detects the OS as cygwin, and the user
    #           needs UNIX behavior from latexmk.
    #     Latexmk has no way of detecting the difference.  The two
    #     situations may even arise for the same user on the same
    #     computer simply by changing the order of directories in the
    #     path environment variable


    ## Configuration parameters: We'll assume native NT executables.
    ## The user should override if they are not.

    # This may fail: perl converts MSWin temp directory name to cygwin
    # format. Names containing this string cannot be handled by native
    # NT executables.
    $tmpdir = "$ENV{TEMP}";

    ## List of possibilities for the system-wide initialization file.
    ## The first one found (if any) is used.
    ## We can stay with MSWin files here, since perl understands them,
    @rc_system_files = ( 'C:/latexmk/LatexMk' );

    $search_path_separator = ';';  # Separator of elements in search_path
    # This is tricky.  The search_path_separator depends on the kind
    # of executable: native NT v. cygwin.
    # So the user will have to override this.

    # For both fptex and miktex, the following makes error messages explicit:
    $latex_silent_switch  = '-interaction=batchmode -c-style-errors';
    $pdflatex_silent_switch  = '-interaction=batchmode -c-style-errors';

    # We will assume that files can be viewed by native NT programs.
    #  Then we must fix the start command/directive, so that the
    #  NT-native start command of a cmd.exe is used.
    # For a pdf-file, "start x.pdf" starts the pdf viewer associated with
    #   pdf files, so no program name is needed:
    $start_NT = "cmd /c start";
    $pdf_previewer = "$start_NT";
    $ps_previewer  = "$start_NT";
    $ps_previewer_landscape  = "$ps_previewer";
    $dvi_previewer  = "$start_NT";
    $dvi_previewer_landscape = "$dvi_previewer";
    # Viewer update methods:
    #    0 => auto update: viewer watches file (e.g., gv)
    #    1 => manual update: user must do something: e.g., click on window.
    #         (e.g., ghostview, MSWIN previewers, acroread under UNIX)
    #    2 => send SIGUSR1 signal (xdvi)
    #    3 => viewer can't update, because it locks the file and the file
    #         cannot be updated.  (acroread under MSWIN)
    $dvi_update_method = 1;
    $ps_update_method = 1;
    $pdf_update_method = 3; # acroread locks the pdf file
    $lpr  = 'NONE lpr';     # Use NONE as flag that I am not implementing this
    $lpr_dvi  = 'NONE lpr_dvi';# Use NONE as flag that I am not implementing this
    $lpr_pdf  = 'NONE lpr_pdf';# Use NONE as flag that I am not implementing this
    # The $pscmd below holds a command to list running processes.  It
    # is used to find the process ID of the viewer looking at the
    # current output file.  The output of the command must include the
    # process number and the command line of the processes, since the
    # relevant process is identified by the name of file to be viewed.
    # Its use is not essential.
    # When the OS is detected as cygwin, there are two possibilities:
    #    a.  Latexmk was run from an NT prompt, but cygwin is in the
    #        path. Then the cygwin ps command will not see commands
    #        started from latexmk.  So we cannot use it.
    #    b.  Latexmk was started within a cygwin environment.  Then
    #        the ps command works as we need.
    # Only the user, not latemk knows which, so we default to not
    # using the ps command.  The user can override this in a
    # configuration file.
    $pscmd = 'NONE pscmd';  # Use NONE as flag that I am not implementing this
    $pid_position = -1;     # offset of PID in output of pscmd.
                            # Negative means I cannot use ps
}
else {
    # Assume anything else is UNIX or clone

    ## Configuration parameters:
    $tmpdir = '/tmp';

    ## List of possibilities for the system-wide initialization file.
    ## The first one found (if any) is used.
    ## Normally on a UNIX it will be in a subdirectory of /opt/local/share or
    ## /usr/local/share, depending on the local conventions.
    ## /usr/local/lib/latexmk/LatexMk is put in the list for
    ## compatibility with older versions of latexmk.
    @rc_system_files =
     ( '/opt/local/share/latexmk/LatexMk',
       '/usr/local/share/latexmk/LatexMk',
       '/usr/local/lib/latexmk/LatexMk' );

    $search_path_separator = ':';  # Separator of elements in search_path
    ## default document processing programs.
    $pdf_previewer = 'start acroread';
    ## The following are corrects for the SUNs at phys.psu.edu:
    #$ps_previewer  = 'start ghostview';
    #$ps_previewer_landscape  = 'start ghostview -swap';
    $ps_previewer  = 'start gv -watch';
    $ps_previewer_landscape  = 'start gv -swap -watch';
    $dvi_previewer  = 'start xdvi';
    $dvi_previewer_landscape = 'start xdvi -paper usr';
    # Viewer update methods:
    #    0 => auto update: viewer watches file (e.g., gv)
    #    1 => manual update: user must do something: e.g., click on window.
    #         (e.g., ghostview, MSWIN previewers, acroread under UNIX)
    #    2 => send SIGUSR1 signal (xdvi)
    #    3 => viewer can't update, because it locks the file and the file
    #         cannot be updated.  (acroread under MSWIN)
    $dvi_update_method = 2;  # xdvi responds to SIGUSR1 to update
    $ps_update_method = 0;   # gv -watch watches the ps file
    $pdf_update_method = 1;  # acroread under unix needs manual update
    $lpr = 'lpr';         # Assume lpr command prints postscript files correctly
    $lpr_dvi  = 'NONE lpr_dvi';# Use NONE as flag that I am not implementing this
    $lpr_pdf  = 'NONE lpr_pdf';# Use NONE as flag that I am not implementing this
    # The $pscmd below holds a command to list running processes.  It
    # is used to find the process ID of the viewer looking at the
    # current output file.  The output of the command must include the
    # process number and the command line of the processes, since the
    # relevant process is identified by the name of file to be viewed.
    # Uses:
    #   1.  In preview_continuous mode, to save running a previewer
    #       when one is already running on the relevant file.
    #   2.  With xdvi in preview_continuous mode, xdvi must be
    #       signalled to make it read a new dvi file.
    #
    # The following works on Solaris, LINUX, HP-UX, IRIX
    # Use -f to get full listing, including command line arguments.
    # Use -u $ENV{CMD} to get all processes started by current user (not just
    #   those associated with current terminal), but none of other users'
    #   processes.
    $pscmd = "ps -f -u $ENV{USER}";
    $pid_position = 1; # offset of PID in output of pscmd; first item is 0.
    if ( $^O eq "linux" ) {
        # Ps on Redhat (at least v. 7.2) appears to truncate its output
        #    at 80 cols, so that a long command string is truncated.
        # Fix this with the --width option.  This option works under
        #    other versions of linux even if not necessary (at least
        #    for SUSE 7.2).
        # However the option is not available under other UNIX-type
        #    systems, e.g., Solaris 8.
        $pscmd = "ps --width 200 -f -u $ENV{USER}";
    }
}

## default parameters
$max_repeat = 5;        # Maximum times I repeat latex.  Normally
                        # 3 would be sufficient: 1st run generates aux file,
                        # 2nd run picks up aux file, and maybe toc, lof which
                        # contain out-of-date information, e.g., wrong page
                        # references in toc, lof and index, and unresolved
                        # references in the middle of lines.  But the
                        # formatting is more-or-less correct.  On the 3rd
                        # run, the page refs etc in toc, lof, etc are about
                        # correct, but some slight formatting changes may
                        # occur, which mess up page numbers in the toc and lof,
                        # Hence a 4th run is conceivably necessary.
                        # At least one document class (JHEP.cls) works
			# in such a way that a 4th run is needed.
                        # We allow an extra run for safety for a
			# maximum of 5. Needing further runs is
			# usually an indication of a problem; further
			# runs may not resolve the problem, and
			# instead could cause an infinite loop.
$clean_ext = "";        # space separated extensions of files that are
                        # to be deleted when doing cleanup, beyond
                        # standard set
$clean_full_ext = "";   # space separated extensions of files that are
                        # to be deleted when doing cleanup_full, beyond
                        # standard set and those in $clean_ext
@cus_dep_list = ();     # Custom dependency list
@default_files = ( '*.tex' );   # Array of LaTeX files to process when
                        # no files are specified on the command line.
                        # Wildcards allowed
                        # Best used for project specific files.
$texfile_search = "";   # Specification for extra files to search for
                        # when no files are specified on the command line
                        # and the @file_list variable is empty.
                        # Space separated, and wildcards allowed.
                        # These files are IN ADDITION to *.tex in current
                        # directory.
                        # This variable is obsolete, and only in here for
                        # backward compatibility.


## default flag settings.
$silent = 0;            # silence latex's messages?
$bibtex_mode = 0;	# is there a bibliography needing bibtexing?
$index_mode = 0;	# is there an index needing makeindex run?
$landscape_mode = 0;	# default to portrait mode
# The following contains a list of extensions for files that may be read in
# during a LaTeX run but that are generated in the previous run.  They should be
# excluded from the dependents, since NORMALLY they are not true source files.
# This list can be overridden in a configuration file if it causes problems.
# The extensions "aux" and "bbl" are always excluded from the dependents,
# because they get special treatment.
@generated_exts = ( 'lof', 'lot', 'toc');
# Which kinds of file do I have requests to make?
# If no requests at all are made, then I will make dvi file
# If particular requests are made then other files may also have to be
# made.  E.g., ps file requires a dvi file
$dvi_mode = 0;          # No dvi file requested
$postscript_mode = 0;           # No postscript file requested
$pdf_mode = 0;          # No pdf file requested to be made by pdflatex
                        # Possible values:
                        #     0 don't create pdf file
                        #     1 to create pdf file by pdflatex
                        #     2 to create pdf file by ps2pdf
                        #     3 to create pdf file by dvipdf
$view = 'default';      # Default preview is of highest of dvi, ps, pdf
$sleep_time = 2;	# time to sleep b/w checks for file changes in -pvc mode
$banner = 0;            # Non-zero if we have a banner to insert
$banner_scale = 220;    # Original default scale
$banner_intensity = 0.95;  # Darkness of the banner message
$banner_message = 'DRAFT'; # Original default message
$cleanup_mode = 0;      # No cleanup of nonessential files.
                        # $cleanup_mode = 0: no cleanup
                        # $cleanup_mode = 1: full cleanup
                        # $cleanup_mode = 2: cleanup except for dvi and ps
                        # $cleanup_mode = 3: cleanup except for dep and aux
$diagnostics = 0;
$dvi_filter = '';	# DVI filter command
$ps_filter = '';	# Postscript filter command

$includes_from_log = 1;  # =1 to work on log file to find dependencies
$force_mode = 0;        # =1 to force processing past errors
$force_include_mode = 0;# =1 to ignore non-existent files when making
                        # dependency files.
$go_mode = 0;           # =1 to force processing regardless of time-stamps
                        # =2 full clean-up first
$preview_mode = 0;
$preview_continuous_mode  = 0;
$printout_mode = 0;     # Don't print the file
@bib_files = ();


# State variables initialized here:

$updated = 0;           # Flags when something has been remade
                        # Used to allow convenient user message in -pvc mode


# Used for some results of parsing log file:
$reference_changed = 0;
$bad_reference = 0;
$bad_citation = 0;


# Get search paths for includes.
$TEXINPUTS = $ENV{'TEXINPUTS'};
if (!$TEXINPUTS) {$TEXINPUTS = '.';}
$BIBINPUTS = $ENV{'BIBINPUTS'};
if (!$BIBINPUTS) { $BIBINPUTS = '.'; }

@psfigsearchpath = ('.');

# Convert search paths to arrays:
# If any of the paths end in '//' then recursively search the
# directory.  After these operations, @BIBINPUTS and @TEXINPUTS should
# have all the directories that need to be searched

@TEXINPUTS = find_dirs1 ($TEXINPUTS);
@BIBINPUTS = find_dirs1 ($BIBINPUTS);

## Read rc files:

# Read first system rc file that is found:
SYSTEM_RC_FILE:
foreach $rc_file ( @rc_system_files )
{
   # print "===Testing for system rc file \"$rc_file\" ...\n";
   if ( -e $rc_file )
   {
      # print "===Reading system rc file \"$rc_file\" ...\n";
      # Read the system rc file
      do "$rc_file";
      last SYSTEM_RC_FILE;
   }
}

# Read user rc file.
$rc_file = "$ENV{'HOME'}/.latexmkrc";
if ( -e $rc_file )
{
  # Read the user rc file
  do "$rc_file";
}

# Read rc file in current directory.
$rc_file = "latexmkrc";
if ( -e $rc_file )
{
  # Read the user rc file
  do "$rc_file";
}

## Process command line args.
@command_line_file_list = ();
$bad_options = 0;
while ($_ = $ARGV[0])
{
  # Make -- and - equivalent at beginning of option:
  s/^--/-/;
  shift;
  if (/^-c$/)        { $cleanup_mode = 2; }
  elsif (/^-commands$/) { &print_commands; exit; }
  elsif (/^-C$/)     { $cleanup_mode = 1; }
  elsif (/^-c1$/)    { $cleanup_mode = 3; }
  elsif (/^-d$/)     { $banner = 1; }
  elsif (/^-dvi$/)   { $dvi_mode = 1; }
  elsif (/^-dvi-$/)  { $dvi_mode = 0; }
  elsif (/^-f$/)     { $force_mode = 1; }
  elsif (/^-f-$/)    { $force_mode = 0; }
  elsif (/^-F$/)     { $force_include_mode = 1; }
  elsif (/^-F-$/)    { $force_include_mode = 0; }
  elsif (/^-g$/)     { $go_mode = 1; }
  elsif (/^-g-$/)    { $go_mode = 0; }
  elsif ( /^-h$/ || /^-help$/ )   { &print_help; }
  elsif (/^-il$/)    { $includes_from_log = 1; }
  elsif (/^-it$/)    { $includes_from_log = 0; }
  elsif (/^-i$/)     { $generate_and_save_includes = 1; }
  elsif (/^-i-$/)    { $generate_and_save_includes = 0; }
  elsif (/^-I$/)     { $force_generate_and_save_includes = 1; }
  elsif (/^-I-$/)    { $force_generate_and_save_includes = 0; }
  elsif (/^-diagnostics/) { $diagnostics = 1; }
  elsif (/^-l$/)     { $landscape_mode = 1; }
  elsif (/^-l-$/)    { $landscape_mode = 0; }
  elsif (/^-p$/)     { $printout_mode = 1;
                       $preview_continuous_mode = 0; # to avoid conflicts
                       $preview_mode = 0;
                     }
  elsif (/^-p-$/)    { $printout_mode = 0; }
  elsif (/^-pdfdvi$/){ $pdf_mode = 3; }
  elsif (/^-pdfps$/) { $pdf_mode = 2; }
  elsif (/^-pdf$/)   { $pdf_mode = 1; }
  elsif (/^-pdf-$/)  { $pdf_mode = 0; }
  elsif (/^-print=(.*)$/) {
      $value = $1;
      if ( $value =~ /^dvi$|^ps$|^pdf$/ ) {
          $print_type = $value;
          $printout_mode = 1;
      }
      else {
          &die_help("Latexmk: unknown print type '$value' in option '$_'");
      }
  }
  elsif (/^-ps$/)    { $postscript_mode = 1; }
  elsif (/^-ps-$/)   { $postscript_mode = 0; }
  elsif (/^-pv$/)    { $preview_mode = 1;
                       $preview_continuous_mode = 0; # to avoid conflicts
                       $printout_mode = 0;
                     }
  elsif (/^-pv-$/)   { $preview_mode = 0; }
  elsif (/^-pvc$/)   { $preview_continuous_mode = 1;
                       $force_mode = 0;    # So that errors do not cause loops
                       $preview_mode = 0;  # to avoid conflicts
                       $printout_mode = 0;
                     }
  elsif (/^-pvc-$/)  { $preview_continuous_mode = 0; }
  elsif (/^-silent$/ || /^-quiet$/ ){ $silent = 1; }
  elsif (/^-v$/ || /^-version$/)   {
      print "\n$version_details. Version $version_num\n";
      exit;
  }
  elsif (/^-verbose$/)  { $silent = 0; }
  elsif (/^-view=default$/) { $view = "default";}
  elsif (/^-view=dvi$/)     { $view = "dvi";}
  elsif (/^-view=ps$/)      { $view = "ps";}
  elsif (/^-view=pdf$/)     { $view = "pdf"; }
  elsif (/^-r$/) {
     if ( $ARGV[0] eq '' ) {
        &die_help( "Latexmk: No RC file specified after -r switch");
     }
     if ( -e $ARGV[0] ) {
	do "$ARGV[0]"; } else {
	die "Latexmk: RC file [$ARGV[0]] does not exist\n";
     }
     shift;
  }
  elsif (/^-bm$/) {
     if ( $ARGV[0] eq '' ) {
	&die_help( "Latexmk: No message specified after -bm switch");
     }
     $banner = 1; $banner_message = $ARGV[0];
     shift;
  }
  elsif (/^-bi$/) {
     if ( $ARGV[0] eq '' ) {
	&die_help( "Latexmk: No intensity specified after -bi switch");
     }
     $banner_intensity = $ARGV[0];
     shift;
  }
  elsif (/^-bs$/) {
     if ( $ARGV[0] eq '' ) {
	&die_help( "Latexmk: No scale specified after -bs switch");
     }
     $banner_scale = $ARGV[0];
     shift;
  }
  elsif (/^-dF$/) {
     if ( $ARGV[0] eq '' ) {
	&die_help( "Latexmk: No dvi filter specified after -dF switch");
     }
     $dvi_filter = $ARGV[0];
     shift;
  }
  elsif (/^-pF$/) {
     if ( $ARGV[0] eq '' ) {
        &die_help( "Latexmk: No ps filter specified after -pF switch");
     }
     $ps_filter = $ARGV[0];
     shift;
  }
  elsif (/^-/) {
     warn "Latexmk: $_ bad option\n";
     $bad_options++;
  }
  else {
     push @command_line_file_list, $_ ;
  }
}

if ( $bad_options > 0 ) {
    &die_help( "Bad options specified" );
}

warn "This is $version_details, version: $version_num.\n",
     "**** Report bugs etc to John Collins <collins at phys.psu.edu>. ****\n"
   unless $silent;

# For backward compatibility, convert $texfile_search to @default_files
# Since $texfile_search is initialized to "", a nonzero value indicates
# that an initialization file has set it.
if ( $texfile_search ne "" ) {
    @default_files = split / /, "*.tex $texfile_search";
}

#Glob the filenames command line if the script was not invoked under a
#   UNIX-like environment.
#   Cases: (1) MS/MSwin native    Glob
#                      (OS detected as MSWin32)
#          (2) MS/MSwin cygwin    Glob [because we do not know whether
#                  the cmd interpreter is UNIXy (and does glob) or is
#                  native MS-Win (and does not glob).]
#                      (OS detected as cygwin)
#          (3) UNIX               Don't glob (cmd interpreter does it)
#                      (Currently, I assume this is everything else)
if ( ($^O eq "MSWin32") || ($^O eq "cygwin") ) {
    @file_list = glob_list(@command_line_file_list);
}
else {
    @file_list = uniq( sort(@command_line_file_list) );
}

# Check we haven't selected mutually exclusive modes.
# Note that -c overides all other options, but doesn't cause
# an error if they are selected.
if (($printout_mode && ( $preview_mode || $preview_continuous_mode ))
    || ( $preview_mode && $preview_continuous_mode ))
{
  # Each of the options -p, -pv, -pvc turns the other off.
  # So the only reason to arrive here is an incorrect inititalization
  #   file, or a bug.
  &die_help( "Latexmk: Conflicting options (print, preview, preview_continuous) selected");
}

if ( @command_line_file_list ) {
    # At least one file specified on command line (before possible globbing).
    if ( !@file_list ) {
        &die_help( "Latexmk: Wildcards in file names didn't match any files");
    }
}
else {
    # No files specified on command line, try and find some
    @file_list = glob_list(@default_files);
    if ( !@file_list ) {
	&die_help( "Latexmk: No file name specified, and I couldn't find any");
    }
}

$num_files = $#file_list + 1;
$num_specified = $#command_line_file_list + 1;

# If selected a preview-continuous mode, make sure exactly one filename was specified
if ($preview_continuous_mode && ($num_files != 1) ) {
    if ($num_specified > 0) {
        &die_help(
          "Latexmk: Need to specify exactly one filename for ".
              "preview-continuous mode\n".
          "    but $num_specified were specified"
        );
    }
    elsif ($num_specified == 1) {
        &die_help(
          "Latexmk: Need to specify exactly one filename for ".
              "preview-continuous mode\n".
          "    but wildcarding produced $num_files files"
        );
    }
    else {
        &die_help(
          "Latexmk: Need to specify exactly one filename for ".
              "preview-continuous mode.\n".
          "    Since none were specified on the command line, I looked for \n".
          "    files in '@default_files'.\n".
          "    But I found $num_files files, not 1."
        );
    }
}


# If landscape mode, change dvips processor, and the previewers:
if ( $landscape_mode )
{
  $dvips = $dvips_landscape;
  $dvi_previewer = $dvi_previewer_landscape;
  $ps_previewer = $ps_previewer_landscape;
}

if ( $silent ) {
    $latex .= " $latex_silent_switch";
    $pdflatex .= " $pdflatex_silent_switch";
    $bibtex .= " $bibtex_silent_switch";
    $dvips .= " $dvips_silent_switch";
}

# Which files do we need to make?
$need_dvi = $need_ps = $need_pdf = 0;
# Which kind of file do we preview?
if ( $view eq "default" ) {
    # If default viewer requested, use "highest" of dvi, ps and pdf
    #    that was requested by user.
    # No explicit request means view dvi.
    $view = "dvi";
    if ( $postscript_mode ) { $view = "ps"; }
    if ( $pdf_mode ) { $view = "pdf"; }
}

# Now check all the requirements that force us to make files.
$need_dvi = $dvi_mode;
$need_ps  = $postscript_mode;
$need_pdf = $pdf_mode;
if ( $preview_continuous_mode || $preview_mode ) {
    if ( $view eq "dvi" ) {$need_dvi = 1; }
    if ( $view eq "ps" )  {$need_ps = 1; }
    if ( $view eq "pdf" ) {
        # Which of the two ways to make pdf files do we use?
        if ( $need_pdf ) {
            # We already have been told how to make pdf files
        }
        else {
            # Use pdflatex route:
            $need_pdf = 1;
        }
    }
}

# What implicit requests are made?
if ( length($dvi_filter) != 0 ) {$need_dvi = 1; }
if ( length($ps_filter) != 0 )  {$need_ps = 1; }
if ( $banner ) { $need_ps = 1; }
# printout => need ps
if ( $printout_mode ) {
    ## May be wrong if print from other kinds of file
    if ( $print_type eq 'dvi' ) {
        $need_dvi = 1;
    }
    elsif ( $print_type eq 'none' ) {
        # Nothing
    }
    elsif ( $print_type eq 'pdf' ) {
        # Will need a pdf file, but there are several methods to make it
        # Respect a previous request, otherwise get pdf by pdflatex
        if ( $need_pdf == 0 ) {
            $need_pdf = 1;
        }
    }
    elsif ( $print_type eq 'ps' ) {
        $need_ps = 1;
    }
    else {
     die "Latexmk: incorrect value \"$print_type\" for type of file to print\n".
         "Allowed values are \"dvi\", \"pdf\", \"ps\", \"none\"\n"
    }
}

# pdf file by ps2pdf => ps file needed.
if ( $need_pdf == 3 ) { $need_dvi = 1; }
if ( $need_pdf == 2 ) { $need_ps = 1; }
# postscript file => dvi file needed.
if ( $need_ps ) { $need_dvi = 1; }

# If no files requested, default to dvi:
if ( ! ($need_dvi || $need_ps || $need_pdf) ) { $need_dvi = 1; }

if ( $need_pdf == 2 ) {
    # We generate pdf from ps.  Make sure we have the correct kind of ps.
    $dvips .= " $dvips_pdf_switch";
}

# Which conversions do we need to make?
$tex_to_dvi = $dvi_to_ps = $ps_to_pdf = $dvi_to_pdf = $tex_to_pdf = 0;
if ($need_dvi) { $tex_to_dvi = 1; }
if ($need_ps) { $dvi_to_ps = 1; }
if ($need_pdf == 1) { $tex_to_pdf = 1; }
if ($need_pdf == 2) { $ps_to_pdf = 1; }
if ($need_pdf == 3) { $dvi_to_pdf = 1; }

# Make convenient forms for lookup.
# Extensions always have period.

# Convert @generated_exts to a hash for ease of look up, with exts preceeded
# by a '.'
%generated_exts = ();
foreach (@generated_exts) {
    $generated_exts{".$_"} = 1;
}

$quell_uptodate_msgs = $silent;
   # Whether to quell informational messages when files are uptodate
   # Will turn off in -pvc mode

# Process for each file.
# The value of $bibtex_mode set in an initialization file may get
# overridden, during file processing, so save it:
$save_bibtex_mode = $bibtex_mode;

FILE:
foreach $filename ( @file_list )
{
    $failure = 0;        # Set nonzero to indicate failure at some point of
                         # a make.  Use value as exit code if I exit.
    $failure_msg = '';   # Indicate reason for failure
    $bibtex_mode = $save_bibtex_mode;
    ## remove extension from filename if was given.
    if ( &find_basename($filename, $root_filename, $texfile_name) )
    {
	if ( $force_mode ) {
	   warn "Latexmk: Could not find file [$texfile_name]\n";
	}
	else {
	    &exit_msg1( "Could not find file [$texfile_name]",
			1);
	}
    }

    if ($cleanup_mode > 0)
    {
        ## Do clean if necessary
        &cleanup_basic;
        if ( $cleanup_mode != 2 ) { &cleanup_dvi_ps_pdf; }
        if ( $cleanup_mode != 3 ) { &cleanup_aux_dep; }
        next FILE;
    }
    if ($go_mode == 2) {
        warn "Latexmk: Removing all generated files\n" unless $silent;
        &cleanup_dvi_ps_pdf;
        &cleanup_aux_dep;
    }
    #Default aux file list:
    @aux_files = ("$root_filename.aux");
    ## Make file. ##
    ## Find includes.
    @includes = ();         # Input files
    %includes_missing = (); # Possible input files, status problematic:
        # filename => [wherefrom, fullname]
        # wherefrom  0: exact name known.
        #            1: name for non-existent file, from logfile, possibly
        #                   bad parse, possibly it's been deleted.
        #            2: not necessarily fullname, from logfile, probably OK
        #                   (graphics file, typically).
        #                   File not found by latexmk.
        #            3: possibly incomplete (pathless, possibly extension-less)
        #                   name from error message in .log file
        #            4: non-found file from .tex file: either non-existent file
        #                   or I didn't find it.
    $read_depend = 0;  # True to read depend file, false to generate it.
    $dep_file = "$root_filename.dep";

    ## Figure out if we read the dependency file or generate a new one.
    if ( ! $force_generate_and_save_includes )
    {
      if ( $generate_and_save_includes )
      {
	if ( -e $dep_file )
	{
	  # Compare timestamp of dependency file and root tex file.
	  $dep_mtime = &get_mtime("$dep_file");
	  $tex_mtime = &get_mtime("$texfile_name");
	  if ( $tex_mtime < $dep_mtime )
	  {
	    $read_depend = 1;
	  }
	}
      }
      elsif ( -e $dep_file )  # If dependency file already exists.
      {
	$read_depend = 1;
      }
    }

    if ( $includes_from_log )
    {
       &parse_log;
       if ( $force_generate_and_save_includes
            || ($generate_and_save_includes && $read_depend == 0 )
          )
       {
          &update_depend_file;
       }
    }
    elsif ( $read_depend )
    {
      # Read the dependency file
      open(dep_file) || die "Latexmk: Couldn't open dependency file [$root_filename.dep]\n";
      while(<dep_file>) { eval; }
      close(dep_file);
    }
    else
    {
      # Generate dependency file.
      &scan_for_includes("$texfile_name");
      &update_depend_file;
    }

#    warn "====@includes===\n";

    #************************************************************

    # Ensure bbl file up-to-date.
    # Also remake the bbl file if there is a bad citation, or if we
    #     use go_mode (which says to remake everything)
    # The call to &make_bbl will also remake the bbl file if it is
    #    out-of-date with respect to the bib files
    # But ignore the return code from make_bbl, since the bbl file depends
    # on the aux file, which may in fact be out of date if the tex file has
    # changed, and we are about to re-latex it.
    if ($bibtex_mode) {
        &make_bbl($bad_citation || $go_mode);
        if ( ($failure > 0) && !$force_mode && ! $preview_continuous_mode) {
            goto END_FIRST;
        }
    }

    # Similarly for ind file.  This is simpler because it only depends
    # on the idx file.
    if ($index_mode) {
        &make_ind($go_mode);
        if ( ($failure > 0) && !$force_mode && ! $preview_continuous_mode) {
            goto END_FIRST;
        }
    }

    $dest_condition_ignore = 0;  #According to this setting
                  # &make_latex_dvi_pdf will or will not examine whether destination
                  # files exist when deciding to make latex
    &make_files($go_mode);
END_FIRST:
    if ( ($failure > 0) && !$force_mode && ! $preview_continuous_mode) {
        &exit_msg1( $failure_msg, $failure );
    }
    if ( $preview_continuous_mode ) {
       &make_preview_continous;
    }
    elsif ( $preview_mode ) {
        &make_preview;
        if ( ($failure > 0) && !$force_mode ) {
           &exit_msg1( $failure_msg, $failure );
        }
    }
    elsif ( $printout_mode) {
        &make_printout;
        if ( ($failure > 0) && !$force_mode ) {
           &exit_msg1( $failure_msg, $failure );
        }
    }
} # end FILES

#************************************************************
#### Subroutines
#************************************************************

#************************************************************
#### Highest level


sub make_files
{
    my $do_build = $_[0];
    my $new_files
       = &find_new_files;
    my $new_deps
       = &make_dependents($do_build);
    if ($failure > 0) {return;}
    if ( ($new_files > 0) || ($new_deps > 0) ) {
	$do_build = 1;
    }
    if ($need_dvi)
    {
        &make_latex_dvi_pdf($do_build, 'dvi');
        if ($failure > 0) {return;}
    }
    if ( $need_ps ) {
        &make_postscript;
        if ($failure > 0) {return;}
    }
    if ( $need_pdf == 1 ) {
        &make_latex_dvi_pdf($do_build, 'pdf');
        if ($failure > 0) {return;}
    }
    if ( $need_pdf == 2 ) {
        &make_pdf2;
        if ($failure > 0) {return;}
    }
    if ( $need_pdf == 3 ) {
        &make_pdf3;
        if ($failure > 0) {return;}
    }
}

#************************************************************

sub make_latex_dvi_pdf
{
  my $do_build = $_[0];
  my $dest_type = $_[1];
  my $dest;
  my $processor;
  if ( $dest_type eq 'dvi' ) {
      $dest = "$root_filename.dvi";
      $processor = $latex;
  } elsif ( $dest_type eq 'pdf' ) {
      $dest = "$root_filename.pdf";
      $processor = $pdflatex;
  } else {
      die("Latexmk: BUG: undefined destination type $dest_type in make_latex_dvi_pdf\n");
  }

  ## get initial last modified times.
  my $tex_mtime = &get_latest_mtime(@includes);
  my $dest_mtime= &get_mtime("$dest");
  my $aux_mtime = &get_mtime("$root_filename.aux");
  my $bib_mtime = &get_latest_mtime(@bib_files);
  my $bbl_mtime = &get_mtime("$root_filename.bbl");
  my $ilg_mtime = &get_mtime("$root_filename.ilg");
  my $ind_mtime = &get_mtime("$root_filename.ind");

  ## - if no destination file (dvi or pdf),
  ##      or .aux older than tex file or bib file or anything they input,
  ##   then run latex.

  #&list_conditions (
  #   $do_build,
  #   !(-e "$root_filename.aux"),
  #   ($aux_mtime < $tex_mtime),
  #   !(-e "$dest"),
  #   ( (-e "$root_filename.bbl") && ($aux_mtime < $bbl_mtime) ),
  #   ($dest_mtime < $tex_mtime),
  #   ( (-e "$root_filename.ilg") && ($aux_mtime < $ilg_mtime) ),
  #   ( (-e "$root_filename.ind") && ($aux_mtime < $ind_mtime) ),
  #   ( $includes_from_log && ! -e "$root_filename.log" )
  #);
  my $dest_bad =  !(-e "$dest") || ($dest_mtime < $tex_mtime) ;
  if ($dest_condition_ignore) {$dest_bad = 0;}
  if ( $do_build
    || !(-e "$root_filename.aux")
    || ($aux_mtime < $tex_mtime)
    || $dest_bad
    || ( (-e "$root_filename.bbl") && ($aux_mtime < $bbl_mtime) )
    || ( (-e "$root_filename.ilg") && ($aux_mtime < $ilg_mtime) )
    || ( (-e "$root_filename.ind") && ($aux_mtime < $ind_mtime) )
    || ( $includes_from_log && ! -e "$root_filename.log" )
   )
   {
      &build_latex_dvi_pdf($processor);
      if ($failure > 0) {return;}
      if ( $dest_type eq 'dvi') {
         &make_dvi_filtered;
         if ($failure > 0) {return;}
      }
   }
   else
   {
       warn "Latexmk: File '$dest' is up to date\n"
          if !$quell_uptodate_msgs;
   }
}

sub list_conditions {
    my $on = 0;
    foreach (@_) {
	if ($_) {$on = 1;}
    }
    if (!$on) {return;}
    print "On conditions: ";
    for ($i = 1; $i <= $#_+1; $i++) {
	if ($_[$i-1]) {print "$i ";}
    }
    print "===\n";
}

#************************************************************

sub build_latex_dvi_pdf {
    # Argument: 0 = processor (e.g., 'latex' or 'pdflatex')
    #
    # I don't need to know whether I run latex or pdflatex!
    #
    # Repeat running latex as many times as needed to resolve cross
    # references, also running bibtex and makeindex as necessary.  The
    # algorithm for determining whether latex needs to be run again is
    # whether certain generated files have changed since the previous
    # run.  A limit (contained in $max_repeat) is applied on the
    # number of runs, to avoid infinite loops in pathological cases or
    # in the case of a bug.  $max_repeat should be at least 4, to
    # allow for the maximum number of repeats under normal
    # circumstances, which is 3, plus one for unusual cases of page
    # breaks moving around.
    #
    # The criterion for needing a rerun is that one or both of the
    # .aux file and the .idx file has changed.  To prove this: observe
    # that reruns are necessary because information that needs to be
    # read in from a file does not correspond to the current
    # conditions.  The relevant files are: the .aux file, and possibly
    # one or more of: the index files, the table of contents file
    # (.toc), the list of figures file (.lof), the list of tables file
    # (.lot).  The information read in is: cross reference
    # definitions, page references, tables of contents, figures and
    # tables.  Note that the list of figures (for example) may itself
    # contain undefined references or incorrect page references.  If
    # there is any incorrectness, changed information will be written
    # to the corresponding file on the current run, and therefore one
    # or more of the auxiliary files will have changed.
    #
    # In fact the lines in the .toc, .lof and .lot files correspond to
    # entries in the .aux file, so it is not necessary to check the
    # .toc, .lof and .lot files (if any).  However WHETHER these files
    # are input by latex is not determined by the aux file, but does
    # affect the output.  It is possible for the tex file to change
    # the state of one of the .toc, .lof, or .lot files between begin
    # required and not required.  The same could happen for other
    # input files.
    #
    # For example, initially suppose no TOC is required, and that all
    # generated files are up-to-date.  The .toc file is either not
    # present or is out-of-date.  Then change the source file so
    # that a TOC is requested.  Run latex; it uses wrong TOC
    # information, but the .aux file might not change.
    #
    # Two possibilities: (a) check .toc, .lof, .lot (possibly etc!)
    # files for changes; (b) check for a change in the list of input
    # files.  We'll choose the second: since it requires less file
    # checking and is more general.  It applies in all situations
    # where the extra auxiliary input files (e.g., .toc) correspond in
    # contents to the state of the .aux file, but only if these files
    # are used.
    #
    # Therefore a correct algorithm is to require a rerun if either of
    # the following is true:
    #
    #    1.  The .aux file has changed from the previous run.
    #    2.  The .idx file has changed from the previous run.
    #    3.  The set of input files has changed from the previous run.
    #
    # Of course, if a previous version of one of these files did not
    # exist (as on a first run), then that implies a rerun is
    # necessary. And if a .aux file or a .idx file is not generated,
    # there is no point in testing it against a previous version.
    #
    # Assume on entry that the .ind and .bbl files are up-to-date with
    # respect to the already existing .idx and .aux files.  This will
    # be the case if latexmk was used to make them.

    my $aux_file;
    my $processor = $_[0];

    my $count_latex = 0;
    my $repeat = 0;

    do {
        # Arrival here means that a new run of latex/pdflatex is requested
        # Assume that the .ind file (if any) corresponds to the .idx file
        #    from the previous run
        # and that the .bbl file (if any) corresponds to the .aux file from
        #    the previous run.

        $repeat = 0;     # Assume no repeat necessary.
	$count_latex++;  # Count the number of passes through latex
        warn "------------\nRun number $count_latex of ",
             "$processor [$texfile_name]\n------------\n";
        foreach $aux_file (@aux_files)
        {
            if ( -e $aux_file ) {
	        warn "Saving old .aux file \"$aux_file\"\n" unless $silent;
                copy_file_and_time ( "$aux_file", "$aux_file.bak");
            }
        }
        if ( (! -e "$root_filename.aux") && (! -e "$root_filename.aux.bak") ) {
            # Arrive here if neither of the .aux and the .aux.bak files exists
            # for the base file.
            # I make minimal .aux.bak file, containing a single line "\relax "
            # This is the same as the aux file generated for a latex file
            # which does not need any cross references, etc.  Generating this
            # .aux.bak file will save a pass through latex on simple files.
            local $aux_bak_file = ">$root_filename.aux.bak";
            open(aux_bak_file) || die "Cannot write file $aux_bak_file\n";
            print aux_bak_file "\\relax \n";
            close(aux_bak_file);
        }
        if ( (-e "$root_filename.aux") && $bibtex_mode) {
           # We have assumed that the bbl file is up-to-date
           #    with respect to the previous aux file.  However
           #    it may be out-of-date with respect to the bib file(s).
           # So run bibtex in this case
           my $bibtex_return_code = &make_bbl(0);
           if ( ($bibtex_return_code eq 2) & !$force_mode )
           {
	       $failure = 1;
               $failure_msg = 'Bibtex reported an error';
               return;
           }
        }

        if ( -e "$root_filename.idx" ) {
	    warn "Saving old .idx file\n" unless $silent;
            copy_file_and_time ( "$root_filename.idx", "$root_filename.idx.bak");
        }

        my @includes_previous = @includes;
        my ($pid, $return) = &Run("$processor $texfile_name");
        $updated = 1;    # Flag that some dependent file has been remade
        if ($return) {
           if (!$force_mode) {
	       $failure = $return;
               $failure_msg = 'Latex encountered an error';
               &aux_restore;
               return;
           }
           elsif ($silent) {
               # User may not have seen error
               warn "====Latex encountered an error: see .log file====\n";
           }
        }
        $return = &parse_log;
        my $aux_changed = 0;
        my $idx_changed = 0;

        if ( $return == 0 )
        {
           $failure = 1;
           $failure_msg = 'Latex failed to generate a log file';
           &aux_restore;
           return;
	}
        if ( $includes_from_log )
        {
           my @aux_files_previous = @aux_files;
           if ( @aux_files ne @aux_files_previous ){
	       $aux_changed = 1;
               warn "List of .aux files has changed. ",
                    "I must run latex again\n"
                    unless $silent;
	   }
        }

        if ( !$aux_changed )
        {
           # Look for changes in the individual aux files.
	   foreach $aux_file (@aux_files)
           {
              if ( -e "$aux_file" ) {
                  if ( &diff ("$aux_file", "$aux_file.bak") ) {
                      warn ".aux file \"$aux_file\" changed.  ",
                           "I must run latex again\n"
                           unless $silent;
                      $aux_changed = 1;
                      last;
                  }
                  else {
                      warn "File \"$aux_file\" has not changed, ",
                           "so it is valid\n" unless $silent;
	          }
	      }
	   }
        }
        if ( (!-e "$root_filename.aux") && (-e "$root_filename.aux.bak") ) {
           warn "No aux file was generated, so I don't need .aux.bak file\n"
              unless $silent;
           unlink ("$root_filename.aux.bak");
        }

        if ( (-e "$root_filename.aux") && $aux_changed && $bibtex_mode) {
            # Note running bibtex only makes sense if the aux file exists.
            # If any aux file has changed, then the citations may
            # have changed and we must run bibtex.
            # The argument to &make_bbl forces this.
            # &make_bbl also checks whether the bbl file is
            #   out-of-date with respect to the bib files.
            my $bibtex_return_code = &make_bbl($aux_changed);
            if ( ($bibtex_return_code eq 2) && !$force_mode )
            {
   	       $failure = 1;
               $failure_msg = 'Bibtex reported an error';
               return;
            }
        }

        if ( -e "$root_filename.idx" ) {
           if ( &diff ("$root_filename.idx", "$root_filename.idx.bak") ) {
               warn "The .idx file changed.  I must run latex again\n"
                   unless $silent;
               # idx file exists and has changed
               #    implies idx file written
               #    implies indexing being used
               $index_mode = 1;
	       $idx_changed = 1;
           } else {
               warn "The .idx file has not changed, so it is valid\n"
                  unless $silent;
           }
           if ($index_mode) {
              my $makeindex_return_code = &make_ind($idx_changed);
              if ( ($makeindex_return_code eq 2) && !$force_mode ) {
                  $failure = 1;
                  $failure_msg = 'Makeindex encountered an error';
                  return;
              }
           }
        } else {
           if ($index_mode) {
              warn "No .idx file was generated, but index_mode is set; ",
                   "I will unset it"
                 unless $silent;
              $index_mode = 0;
           }
           if ( -e "$root_filename.idx.bak") {
              warn "No idx file was generated. ",
                   "So I delete unneeded .idx.bak file\n"
                 unless $silent;
              unlink ("$root_filename.idx.bak");
           }
        }

        if ( @includes ne @includes_previous ) {
            warn "The set of input files changed.  I must run latex again\n"
              unless $silent;
        }

        if ( $aux_changed
             || $idx_changed
             ||( @includes ne @includes_previous )
           ) {
	    $repeat = 1;
        }

        if ( $count_latex ge $max_repeat ) {
           # Avoid infinite loop by having a maximum repeat count
           # Getting here represents some kind of weird error.
           if ($repeat ) {
              warn "Maximum runs of latex reached ",
                   "without correctly resolving cross references\n";
           }
           $repeat = 0;
        }
    } until ($repeat == 0);

    if ($bibtex_mode) {
        my $retcode = &check_for_bibtex_warnings;
        if ($retcode == 1) {
           print "See $root_filename.blg for details\n";
        }
        elsif ($retcode == 2) {
           return;
        }
    }

    if (!$force_mode && $bad_reference) {
        $failure = 1;
        $failure_msg = 'Latex could not resolve all references';
        return;
    }

     if (!$force_mode && $bad_citation) {
        $failure = 1;
        $failure_msg = 'Latex could not resolve all citations or labels';
        return;
    }
}

#************************************************************

# Finds the basename of the root file
# Arguments:
#  1 - Filename to breakdown
#  2 - Where to place base file
#  3 - Where to place tex file
#  Returns non-zero if tex file does not exist
#
# The rules for determining this depend on the implementation of TeX.
# The variable $extension_treatment determines which rules are used.

sub find_basename
{
  local($given_name, $base_name, $ext, $path, $tex_name);
  $given_name = $_[0];
  if ( "$extension_treatment" eq "miktex_old" ) {
       # Miktex v. 1.20d:
       #   1. If the filename has an extension, then use it.
       #   2. Else append ".tex".
       #   3. The basename is obtained from the filename by
       #      removing the path component, and the extension, if it
       #      exists.  If a filename has a multiple extension, then
       #      all parts of the extension are removed.
       #   4. The names of generated files (log, aux) are obtained by
       #      appending .log, .aux, etc to the basename.  Note that
       #      these are all in the CURRENT directory, and the drive/path
       #      part of the originally given filename is ignored.
       #
       #   Thus when the given filename is "\tmp\a.b.c", the tex
       #   filename is the same, and the basename is "a".

       ($base_name, $path, $ext) = fileparse ($given_name, '\..*');
       if ( "$ext" eq "") { $tex_name = "$given_name.tex"; }
       else { $tex_name = $given_name; }
       $_[1] = $base_name;
       $_[2] = $tex_name;
  }
  elsif ( "$extension_treatment" eq "unix" ) {
       # unix (at least web2c 7.3.1) =>
       #   1. If filename.tex exists, use it,
       #   2. else if filename exists, use it.
       #   3. The base filename is obtained by deleting the path
       #      component and, if an extension exists, the last
       #      component of the extension, even if the extension is
       #      null.  (A name ending in "." has a null extension.)
       #   4. The names of generated files (log, aux) are obtained by
       #      appending .log, .aux, etc to the basename.  Note that
       #      these are all in the CURRENT directory, and the drive/path
       #      part of the originally given filename is ignored.
       #
       #   Thus when the given filename is "/tmp/a.b.c", there are two
       #   cases:
       #      a.  /tmp/a.b.c.tex exists.  Then this is the tex file,
       #          and the basename is "a.b.c".
       #      b.  /tmp/a.b.c.tex does not exist.  Then the tex file is
       #          "/tmp/a.b.c", and the basename is "a.b".

      if ( -e "$given_name.tex" ) {
         $tex_name = "$given_name.tex";
      }
      else {
         $tex_name = "$given_name";
      }
      ($base_name, $path, $ext) = fileparse ($tex_name, '\.[^\.]*');
      $_[1] = $base_name;
      $_[2] = $tex_name;
  }
  else {
     die "Latexmk: Incorrect configuration gives \$extension_treatment=",
         "\"$extension_treatment\"\n";
  }
   if ($diagnostics) {
      print "Given=\"$given_name\", tex=\"$tex_name\", base=\"$base_name\"\n";
  }
  return ! -e $tex_name;
}

#************************************************************

sub make_bbl {
# If necessary, make bbl file.  Assume bibtex mode on.
# Force run if first argument is non-zero.
# Return 0 if nothing made,
#        1 if bbl file made,
#        2 if bibtex reported an error
#        3 if .blg file couldn't be opened
#        4 if there was another error
   my $bib_mtime = &get_latest_mtime(@bib_files);
   my $bbl_mtime = &get_mtime("$root_filename.bbl");
   ## if no .bbl or .bib changed since last bibtex run, run bibtex.
   if ( !-e "$root_filename.aux" )
   {
      # bibtex reads aux file, so if there is no aux file, there is
      # nothing to do
       return 0;
   }
   if (($_[0] != 0)
       || !(-e "$root_filename.bbl")
       || ($bbl_mtime < $bib_mtime)
       )
   {
       warn "------------\nRunning $bibtex [$root_filename]",
            "\n------------\n"
         unless $silent;
       my ($pid, $return) = &Run("$bibtex $root_filename");
       $updated = 1;
       $bbl_mtime = &get_mtime("$root_filename.bbl");
       if ( $return != 0 )
          {  return 2; }
       $return = &check_for_bibtex_errors;
       if ( $return == 0 )
          { return 1;}
       elsif ( $return == 1 )
          { return 2;}
       elsif ( $return == 2 )
          { return 3;}
       else
          { return 4; }
   }
   else
   { return 0; }
}

#************************************************************

sub make_ind {
# If necessary, make ind file.  Assume makeindex mode on.
# Force run if first argument is non-zero.
# Return 0 if nothing made,
#        1 if ind file made,
#        2 if makeindex reported an error
   if ( !-e "$root_filename.idx" )
   {
      # makeindex reads idx file, so if there is no idx file, there is
      # nothing to do
      return 0;
   }
   if ( ($_[0] != 0) || !(-e "$root_filename.ind") )
   {
      warn "------------\nRunning $makeindex [$root_filename]",
           "\n------------\n"
         unless $silent;
      my ($pid, $return) = &Run("$makeindex $root_filename");
      $updated = 1;
      if ($return)
      {
	  $failure = 1;
          $failure_msg = 'Problem with makeindex';
          return 2;
      }
      else
      { return 1; }
   }
   else
   { return 0; }
}

#************************************************************

sub find_new_files
{
    my @new_includes = ();
MISSING_FILE:
    foreach my $missing (sort keys %includes_missing) {
       my ($base, $path, $ext) = fileparse ($missing, '\.[^\.]*');
       if ( -e "$missing.tex" ) {
	   push @new_includes, "$missing.tex";
# It's probably best to try all possibilities, since
# we don't know which one applies.  So go on to next case.
#           next MISSING_FILE;
       }
       if ( -e $missing ) {
	   push @new_includes, $missing;
#           next MISSING_FILE;
       }
       if ( $ext ne "" ) {
           foreach my $dep (@cus_dep_list){
              my ($fromext,$toext) = split(' ',$dep);
              if ( ( "$ext" eq ".$toext" )
                   && ( -e "$path$base.$fromext" )
		  )  {
                  # Source file for the missing file exists
                  # So we have a real include file, and it will be made
                  # next time by &make_dependents
                  push @new_includes, $missing ;
#                  next MISSING_FILE;
              }
              # no point testing the $toext if the file doesn't exist.
	   }
       }
       else {
           # $_ doesn't exist, $_.tex doesn't exist,
           # and $_ doesn't have an extension
           foreach my $dep (@cus_dep_list){
              my ($fromext,$toext) = split(' ',$dep);
              if ( -e "$path$base.$fromext" ) {
                  # Source file for the missing file exists
                  # So we have a real include file, and it will be made
                  # next time by &make_dependents
                  push @new_includes, "$path$base.$toext" ;
#                  next MISSING_FILE;
              }
              if ( -e "$path$base.$toext" ) {
                  # We've found the extensionfor the missing file,
                  # and the file exists
                  push @new_includes, "$path$base.$toext" ;
#                  next MISSING_FILE;
              }
	   }
       }
    } # end MISSING_FILES

    @new_includes = uniq( sort(@new_includes) );
    @includes = uniq( sort(@includes, @new_includes) );

    my $found = $#new_includes + 1;
    if ( $diagnostics && ( $found > 0 ) ) {
	warn "Detected previously missing files:\n";
        foreach (@new_includes) {
            warn "   '$_'\n";
	}
    }
    return $found;
}

#************************************************************

sub make_dependents
{
# Usage: make_dependents(build)
# First argument = 1 => rebuild unconditionally
#                  0 => rebuild only if dest is out-of-date
# Return 0 if nothing made, 1 if something made
  my $build = shift;
  my $makes = 0;     # Count of makes done
FILE:
  foreach my $file (@includes)
  {
     my ($base_name, $path, $toext) = fileparse ($file, '\.[^\.]*');
     $base_name = $path.$base_name;
     if ( $toext eq "" ) {next FILE;}
     $toext =~ s/^\.//;
DEP:
     foreach my $dep ( @cus_dep_list )
     {
        my ($fromext,$proptoext,$must,$func_name) = split(' ',$dep);
        if ( $toext eq $proptoext )
	{
	   # Found match of rule
	   if ( -e "$base_name.$fromext" )
	   {
              # From file exists, now check if it is newer
  	      if (( ! (-e "$base_name.$toext" ))
                  || $build
		  || ( &get_mtime("$base_name.$toext")
                        < &get_mtime("$base_name.$fromext")
                     )
                 )
              {
	         warn "------------\nRunning $func_name [$base_name]\n------------\n"
                    unless $silent;
	         my $return = &$func_name($base_name);
                 $updated = 1;
	         if ( !$force_mode && $return )
	         {
                    $failure = $return;
                    $failure_msg = "$func_name encountered an error";
                    last FILE;
	         }
                 else {
                    $makes++;
		 }
	     }
	  }
	  else
	  {  # Source file does not exist
             # Perhaps the rule is not to be applied.
	     if ( !$force_mode && ( $must != 0 ))
	     {
                $failure = 1;
                $failure_msg = "File '$base_name.$fromext' does not exist ".
                               "to build '$base_name.$toext'";
                last FILE;
	     }
	  } #
       } # End of Rule found
     } # End DEP
  } # End FILE
  return ($makes>0 ? 1 : 0);
} # End sub make_dependents

#************************************************************

sub make_dvi_filtered
{
  my $dvi_file = "$root_filename.dvi";
  return if ( length($dvi_filter) == 0 );
  if ( ! -e $dvi_file ) {
       warn "Dvi file \"$dvi_file\" has not been made, so I cannot filter it\n";
       return;
  }
  warn "------------\nRunning $dvi_filter [$root_filename]\n------------\n"
     unless $silent;
  &Run("$dvi_filter < $dvi_file > $root_filename.dviF");
  $updated = 1;
}

#************************************************************

sub make_pdf2
{
    my $ps_file;
    my $pdf_file;

    if ( length($ps_filter) == 0 )
        {$ps_file = "$root_filename.ps";}
    else
        {$ps_file = "$root_filename.psF";}
    $pdf_file = "$root_filename.pdf";

    my $ps_mtime = &get_mtime("$ps_file");
    my $pdf_mtime = &get_mtime("$pdf_file");
    if ( ! -e $ps_file ) {
	warn "Postscript file \"$ps_file\" has not been made\n";
        return;
    }
    if ((! -e "$pdf_file")
        ||( $pdf_mtime < $ps_mtime )
       )
    {
        warn "------------\nRunning $ps2pdf [$root_filename]\n------------\n"
          unless $silent;
        &Run("$ps2pdf  $ps_file $pdf_file");
        $updated = 1;
    }
    else
    {
        warn "Latexmk: File '$pdf_file' is up to date\n" if !$quell_uptodate_msgs;
    }
}

#************************************************************

sub make_pdf3
{
    my $dvi_file;
    my $pdf_file;

    if ( length($dvi_filter) == 0 )
        {$dvi_file = "$root_filename.dvi";}
    else
        {$dvi_file = "$root_filename.dviF";}
    $pdf_file = "$root_filename.pdf";

    my $dvi_mtime = &get_mtime("$dvi_file");
    my $pdf_mtime = &get_mtime("$pdf_file");
    if ( ! -e $dvi_file ) {
	warn "Dvi file \"$dvi_file\" has not been made, so I cannot convert it to pdf\n";
        return;
    }
    if ((! -e "$pdf_file")
        ||( $pdf_mtime < $dvi_mtime )
       )
    {
        warn "------------\nRunning $dvipdf [$root_filename]\n------------\n"
           unless $silent;
        &Run("$dvipdf  $dvi_file $pdf_file");
        $updated = 1;
    }
    else
    {
        warn "Latexmk: File '$pdf_file' is up to date\n" if !$quell_uptodate_msgs;
    }
}

#************************************************************

sub make_printout
{
  my $ext = '';      # extension of file to print
  my $command = '';  # command to print it
  if ( $print_type eq 'dvi' ) {
      if ( length($dvi_filter) == 0 )
      {
	$ext = '.dvi';
      }
      else
      {
	$ext = '.dviF';
      }
      $command = $lpr_dvi;
  }
  elsif ( $print_type eq 'pdf' ) {
      $ext = '.pdf';
      $command = $lpr_pdf;
  }
  elsif ( $print_type eq 'ps' ) {
      if ( length($ps_filter) == 0 )
      {
	$ext = '.ps';
      }
      else
      {
	$ext = '.psF';
      }
      $command = $lpr;
  }
  elsif ( $print_type eq 'none' ) {
      warn "------------\nPrinting is configured off\n------------\n";
      return;
  }
  else
  {
     die "Latexmk: incorrect value \"$print_type\" for type of file to print\n".
         "Allowed values are \"dvi\", \"pdf\", \"ps\", \"none\"\n"
  }
  my $file = $root_filename.$ext;
  if ( ! -e $file ) {
      warn "File \"$file\" has not been made, so I cannot print it\n";
      return;
  }
  warn "------------\nPrinting using $command $file\n------------\n"
     unless $silent;
  &Run("$command $file");
}

#************************************************************

sub make_postscript
{
  my $tmpfile;
  my $header;
  my $dvi_file;

  # Figure out the dvi file name
  if ( length($dvi_filter) == 0 )
  {
    $dvi_file = "$root_filename.dvi";
  }
  else
  {
    $dvi_file = "$root_filename.dviF";
  }

  if ( ! -e $dvi_file ) {
      warn "Dvi file \"$dvi_file\" has not been made, so I cannot convert it to postscript\n";
      return;
  }

  # Do banner stuff
  if ( $banner )
  {
    ## Make temp banner file
#    local(*INFILE,*OUTFILE,$count);
    local(*OUTFILE,$count);

    $tmpfile = "$tmpdir/latexmk.$$";
    $count = 0;
    while ( -e $tmpfile )
    {
      $count = $count + 1;
      $tmpfile = "$tmpdir/latexmk.$$.$count";
    }
    if ( ! open(OUTFILE, ">$tmpfile") ) {
      die "Latexmk: Could not open temporary file [$tmpfile]\n"; }
    print OUTFILE "userdict begin /bop-hook{gsave 200 30 translate\n";
    print OUTFILE "65 rotate /Times-Roman findfont $banner_scale scalefont setfont\n";
    print OUTFILE "0 0 moveto $banner_intensity setgray ($banner_message) show grestore}def end\n";
    close(OUTFILE);
    $header = "-h $tmpfile";
  }
  else
  {
    $header = '';
  }

  my $ps_mtime = &get_mtime("$root_filename.ps");
  my $dvi_mtime = &get_mtime("$dvi_file");
  if ((! -e "$root_filename.ps")
      ||( $ps_mtime < $dvi_mtime )
     )
  {
      warn "------------\nRunning $dvips [$root_filename]\n------------\n"
         unless $silent;
      &Run("$dvips $header $dvi_file -o $root_filename.ps");
      ## Do we have postscript filtering?
      if ( length($ps_filter) != 0 )
      {
         warn "------------\nRunning $ps_filter [$root_filename]\n------------\n"
            unless $silent;
         &Run("$ps_filter < $root_filename.ps > $root_filename.psF");
      }
      $updated = 1;
  }
  else
  {
      warn "Latexmk: File '$root_filename.ps' is up to date\n" if !$quell_uptodate_msgs;
  }

  if ( $banner )
  {
    unlink("$tmpfile");
  }

}

#************************************************************
# run appropriate previewer.

sub make_preview
{
  my $ext;
  my $viewer;
  if ( $view eq 'dvi' )
  {
     $viewer = $dvi_previewer;
     $ext = '.dvi';
     if ( length($dvi_filter) != 0 )
     {
       $ext = '.dviF';
     }
  }
  elsif ( $view eq 'ps' )
  {
    $viewer = $ps_previewer;
    $ext = '.ps';
    if ( length($ps_filter) != 0 )
    {
      $ext = '.psF';
    }
  }
  elsif ( $view eq 'pdf' )
  {
    $viewer = $pdf_previewer;
    $ext = '.pdf';
  }
  else
  {
      die "Latexmk: BUG: No preview method defined\n";
  }

  my $view_file = "$root_filename$ext";

  if ( ! -e $view_file ) {
      warn "File \"$view_file\" has not been made, so I cannot view it\n";
      return;
  }
  warn "------------\nStarting previewer: $viewer $view_file\n------------\n"
     unless $silent;
  my ($pid, $return) = &Run ("$viewer $view_file");
  if ($return){
    warn "Latexmk: Could not start previewer [$viewer $view_file]";
  }
#  exit;
}

#************************************************************

sub make_preview_continous
{

  # How do we persuade viewer to update.  Default is to do nothing.
  my $viewer_update_method = 0;
  # Extension of file:
  my $ext;
  $quell_uptodate_msgs = 1;
  if ( $view eq 'dvi' )
  {
     $viewer = $dvi_previewer;
     $viewer_update_method = $dvi_update_method;
     $ext = '.dvi';
     if ( length($dvi_filter) != 0 )
     {
       $ext = '.dviF';
     }
  }
  elsif ( $view eq 'ps' )
  {
     $viewer = $ps_previewer;
     $viewer_update_method = $ps_update_method;
     $ext = '.ps';
     if ( length($ps_filter) != 0 )
     {
        $ext = '.psF';
     }
  }
  elsif ( $view eq 'pdf' )
  {
     $viewer_update_method = $pdf_update_method;
     $viewer = $pdf_previewer;
     $ext = '.pdf';
  }
  else
  {
      die "Latexmk: BUG: No preview method defined\n";
  }


  # Viewer information:
  my $viewer_running = 0;    # No viewer running yet
  my $view_file = "$root_filename$ext";
  my $viewer_process = 0;    # default: no viewer process number known

  $dest_condition_ignore = 1;   #make_latex_dvi-pdf will ignore destination
                                #in make rule. If the destination file does not
                                #exists, it is because of an error.
  # Loop forever, rebuilding .dvi and .ps as necessary.
  my $first_time = 1;
  while ( 1 ) {
     # Wait to run a viewer until we know we have the view file
     # If there are errors in the initial run, the
     if ( (!$viewer_running) && (-e $view_file) ) {
        # note, we only launch a previewer if one isn't already running...
        # But I only know how to do this under UNIX
        $viewer_process = &find_process_id(  $view_file );
        if ( $viewer_process ) {
           warn "Previewer is already running\n"
              if $first_time && !$silent;
        } else {
           warn "I have not found a previewer that is already running. \n",
                "So I will start it: $viewer $view_file\n------------\n"
              unless $silent;
           my $retcode;
           ($viewer_process, $retcode)
              = &Run ("start $viewer $root_filename$ext");
           if ( $retcode != 0 ) {
              if ($force_mode) {
                 warn "I could not run previewer\n";
              }
              else {
                 &exit_msg1( "I could not run previewer", $retcode);
	      }
           }
           else {
              $viewer_running = 1;
	   } # end analyze result of trying to run viewer
       } # end if $viewer_process
     } # end start viewer
     if ( $first_time || $updated ) {
        print "\n=== Watching for updated files. Use ctrl/C to stop ...\n";
        $first_time = 0;
     }
     $updated = 0;
     $failure = 0;
     sleep($sleep_time);
     &make_files;
     if ( ($failure <= 0)
          && ($viewer_process != 0)
          && $updated
          && ($viewer_update_method == 2)
        )
     {
        if ( $diagnostics ) {
	    print "I am signalling viewer, process ID $viewer_process\n";
        }
	kill 'USR1', $viewer_process;
     }
  } #end infinite_loop
} #end sub make_preview_continuous

#************************************************************

# cleanup_basic
# - erases basic set of generated files, exits w/ no other processing.
#   (all but aux, dep, dvi, pdf, and ps),
#   and also texput.log, and files with extensions in $clean_ext

sub cleanup_basic
{
# Basic set:
  unlink("$root_filename.aux.bak");
  unlink("$root_filename.bbl");
  unlink("$root_filename.blg");
  unlink("$root_filename.log");
  unlink("$root_filename.ind");
  unlink("$root_filename.idx");
  unlink("$root_filename.idx.bak");
  unlink("$root_filename.ilg");
  unlink("$root_filename.toc");
  unlink("$root_filename.toc.bak");
  unlink("$root_filename.lof");
  unlink("$root_filename.lot");
  unlink("texput.log");

  # Do any other file extensions specified
  foreach $ext (split(' ',$clean_ext))
  {
    unlink("$root_filename.$ext");
  }
}


#************************************************************
# cleanup_dvi_ps_pdf
# - erases generated dvi, ps, and pdf files (and others specified in
#   $cleanup_full_ext),
#   and also texput.dvi, and files with extensions in $clean_full_ext

sub cleanup_dvi_ps_pdf
{
  unlink("$root_filename.dvi");
  unlink("$root_filename.pdf");
  unlink("$root_filename.ps");
  unlink("$root_filename.dviF");
  unlink("$root_filename.psF");
  unlink("texput.dvi");
  # Do any other file extensions specified
  foreach $ext (split(' ',$clean_full_ext))
  {
    unlink("$root_filename.$ext");
  }
}


#************************************************************
# cleanup_aux_dep
# - erases generated aux and dep files, and also texput.aux

sub cleanup_aux_dep
{
  unlink("$root_filename.aux");
  unlink("$root_filename.dep");
  unlink("texput.aux");
  # .aux files are also made for \include'd files
  foreach my $include (@includes) {
     $include =~ s/\.[^\.]*$/.aux/;
     unlink($include);
  }
}


#************************************************************

sub aux_restore {
   warn "Latexmk: restoring last $root_filename.aux file\n";
   # But don't copy the time from the aux.bak file
   # So the aux file will look up-to-date
   copy_file_keep_time( "$root_filename.aux.bak", "$root_filename.aux" );
}

#************************************************************

sub exit_msg1
{
  # exit_msg1( error_message, retcode [, action])
  #    1. display error message
  #    2. if action set, then restore aux file
  #    3. exit with retcode
  warn "\n------------\n";
  warn "Latexmk: $_[0].\n";
  warn "-- Use the -f option to force complete processing.\n";
  if ($_[2])
  {
      &aux_restore;
  }
  my $retcode = $_[1];
  if ($retcode >= 256) {
     # Retcode is the kind returned by system from an external command
     # which is 256 * command's_retcode
     $retcode /= 256;
  }
  exit $retcode;
}

#************************************************************

sub die_help
# Die giving diagnostic from arguments and how to get help.
{
    die "\n@_\nUse\n    latexmk -help\nto get usage information\n";
}


#************************************************************

sub print_help
{
  print
  "Latexmk $version_num: Automatic LaTeX document generation routine\n\n",
  "Usage: latexmk [latexmk_options] [filename ...]\n\n",
  "  Latexmk_options:\n",
  "   -bm <message> - Print message across the page when converting to postscript\n",
  "   -bi <intensity> - Set contrast or intensity of banner\n",
  "   -bs <scale> - Set scale for banner\n",
  "   -commands  - list commands used by latexmk for processing files\n",
  "   -c     - clean up (remove) all nonessential files, except\n",
  "            dvi, ps and pdf files\n",
  "   -C     - clean up (remove) all nonessential files\n",
  "            including aux, dep, dvi, postscript and pdf files\n",
  "   -c1    - clean up (remove) all nonessential files,\n",
  "            including dvi, pdf and ps files, but excluding aux and dep files \n",
  "   -d     - Print `DRAFT' across the page when converting to postscript\n",
  "   -dF <filter> - Filter to apply to dvi file\n",
  "   -dvi   - generate dvi\n",
  "   -dvi-  - turn off required dvi\n",
  "   -f     - force continued processing past errors\n",
  "   -f-    - turn off forced continuing processing past errors\n",
  "   -F     - Ignore non-existent files when making dependencies\n",
  "   -F-    - Turn off -F\n",
  "   -g     - process regardless of file timestamps\n",
  "   -g-    - Turn off -g\n",
  "   -h     - print help\n",
  "   -help - print help\n",
  "   -i     - rescan for input if dependency file older than tex file\n",
  "   -i-    - Turn off -i\n",
  "   -il    - make list of input files by parsing log file\n",
  "   -it    - make list of input files by parsing tex file\n",
  "   -I     - force rescan for input files\n",
  "   -I-    - Turn off -I\n",
  "   -l     - force landscape mode\n",
  "   -l-    - turn off -l\n",
  "   -pdf   - generate pdf by pdflatex\n",
  "   -pdfdvi - generate pdf by dvipdf\n",
  "   -pdfps - generate pdf by ps2pdf\n",
  "   -pdf-  - turn off pdf\n",
  "   -ps    - generate postscript\n",
  "   -ps-   - turn off postscript\n",
  "   -pF <filter> - Filter to apply to postscript file\n",
  "   -p     - print document after generating postscript.\n",
  "            (Can also print .dvi or .pdf files -- see documentation)\n",
  "   -print=dvi - print dvi file\n",
  "   -print=ps  - print ps file\n",
  "   -print=pdf - print pdf file\n",
  "   -pv    - preview document.  (Side effect turn off continuous preview)\n",
  "   -pv-   - turn off preview mode\n",
  "   -pvc   - preview document and continuously update.  (This also turns\n",
  "                on force mode, so errors do not cause latexmk to stop.)\n",
  "            (Side effect: turn off ordinary preview mode.)\n",
  "   -pvc-  - turn off -pvc\n",
  "   -r <file> - Read custom RC file\n",
  "   -silent  - silence progress messages from called programs\n",
  "   -v     - display program version\n",
  "   -verbose - display usual progress messages from called programs\n",
  "   -version      - display program version\n",
  "   -view=default - viewer is default (dvi, ps, pdf)\n",
  "   -view=dvi     - viewer is for dvi\n",
  "   -view=ps      - viewer is for ps\n",
  "   -view=pdf     - viewer is for pdf\n",
  "   filename = the root filename of LaTeX document\n",
  "\n",
  "-p, -pv and -pvc are mutually exclusive\n",
  "-h, -c and -C overides all other options.\n",
  "-pv and -pvc require one and only one filename specified\n",
  "All options can be introduced by '-' or '--'.  (E.g., --help or -help.)\n",
  "Contents of RC file specified by -r overrides options specified\n",
  "  before the -r option on the command line\n";

  exit;
}

#************************************************************
sub print_commands
{
  warn "Commands used by latexmk:\n",
       "   To run latex, I use \"$latex\"\n",
       "   To run pdflatex, I use \"$pdflatex\"\n",
       "   To run bibtex, I use \"$bibtex\"\n",
       "   To run makeindex, I use \"$makeindex\"\n",
       "   To make a ps file from a dvi file, I use \"$dvips\"\n",
       "   To make a ps file from a dvi file with landscape format, ",
           "I use \"$dvips_landscape\"\n",
       "   To make a pdf file from a dvi file, I use \"$dvipdf\"\n",
       "   To make a pdf file from a ps file, I use \"$ps2pdf\"\n",
       "   To view a pdf file, I use \"$pdf_previewer\"\n",
       "   To view a ps file, I use \"$ps_previewer\"\n",
       "   To view a ps file in landscape format, ",
            "I use \"$ps_previewer_landscape\"\n",
       "   To view a dvi file, I use \"$dvi_previewer\"\n",
       "   To view a dvi file in landscape format, ",
            "I use \"$dvi_previewer_landscape\"\n",
       "   To print a ps file, I use \"$lpr\"\n",
       "   To print a dvi file, I use \"$lpr_dvi\"\n",
       "   To print a pdf file, I use \"$lpr_pdf\"\n",
       "   To find running processes, I use \"$pscmd\", ",
          "and the process number is at position $pid_position\n";
   warn "Notes:\n",
        "  Command starting with \"start\" is run detached\n",
        "  Command that is just \"start\" without any other command, is\n",
        "     used under MS-Windows to run the command the operating system\n",
        "     has associated with the relevant file.\n",
        "  Command starting with \"NONE\" is not used at all\n";
}

#************************************************************
#### Tex-related utilities


# check for citation which bibtex didnt find.

sub check_for_bibtex_errors
# return 0: OK, 1: bibtex error, 2: could not open .blg file.
{
  my $log_name = "$root_filename.blg";
  my $log_file = 0;
  my $retcode = open( $log_file, "<$log_name" );
  if ( $retcode == 0) {
     if ( !$force_mode ) {
        $failure = 1;
        $failure_msg = "Could not open bibtex log file for error check";
     }
     warn "Could not open bibtex log file for error check\n";
     return 2;
  }
  $retcode = 0;
  while (<$log_file>)
  {
#    if (/Warning--/) { return 1; }
    if (/error message/)
    {
       $retcode = 1;
       last;
    }
  }
  close $log_file;
  return $retcode;
}

#************************************************************
# check for bibtex warnings

sub check_for_bibtex_warnings
# return 0: OK, 1: bibtex warnings, 2: could not open .blg file.
{
  my $log_name = "$root_filename.blg";
  my $log_file = 0;
  my $retcode = open( $log_file, "<$log_name" );
  if ( $retcode == 0 )
  {
      $failure = 1;
      $failure_msg = "Could not open bibtex log file for warning check";
      warn "Latexmk: $failure_msg\n";
      return 2;
  }
  my $have_warning = 0;
  while (<$log_file>)
  {
    if (/Warning--/) { print "Bibtex warning: $_"; $have_warning = 1}
    if (/error message/) { print "Bibtex error: $_"; $have_warning = 1}
  }
  close $log_file;
  return $have_warning;
}

#************************************************************
# - looks recursively for included & inputted and psfig'd files and puts
#   them into @includes.
# - note only primitive comment removal: cannot deal with escaped %s, but then,
#	when would they occur in any line important to latexmk??

sub scan_for_includes
{
  my $texfile_name = $_[0];
  warn "-----Scanning [$texfile_name] for input files etc ... -----\n";
  &scan_for_includes_($texfile_name);
  ## put root tex file into list of includes.
  push @includes, $texfile_name;
}

sub scan_for_includes_
{
  local(*FILE,$orig_filename);
##JCC
  local($ignoremode,$line);
  $ignoremode = 0;
  $line = 0;
  if (!open(FILE,$_[0]))
  {
    warn "Latexmk: could not open input file [$_[0]]\n";
    return;
  }
LINE:
  while(<FILE>)
  {
    $line = $line + 1;

    if ( /^\s*(%#.*)\s*$/ )
    {
       $_ = $1;
       ##warn "======Metacommand \"$_\"\n";
       if ( /%#{}end.raw/ || /%#{latexmk}end.ignore/ )
       {
	   $ignoremode = 0;
           warn "  Ignore mode off, at line $line in file $_[0].\n";
       }
       elsif ( $ignoremode == 1 )
       {
           # In ignore mode only end.raw, etc metacommands are recognized.
	   next LINE;
       }
       elsif ( /%#{}raw/ || /%#{}begin.raw/ ||
            /%#{latexmk}ignore/ || /%#{latexmk}begin.ignore/ )
       {
	   $ignoremode = 1;
           warn "  Ignore mode on, at line $line in file $_[0].\n";
       }
       elsif ( /%#{latexmk}include.file[\040\011]+([^\040\011\n]*)/
               || /%#{latexmk}input.file[\040\011]+([^\040\011\n]*)/ )
       {
          push @includes, $1;
          warn "  Adding input file \"$1\" at line $line in file $_[0].\n";
       }
       else
       {
         # Unrecognized metacommands are, by specification, to be ignored.
	   warn "Unrec. \"$_\"\n";
       }
       next LINE;
    }
    if ( $ignoremode == 1 )
    {
	##warn "Skipping a line:\n  $_";
        next LINE;
    }

    ($_) = split('%',$_);		# primitive comment removal

    if (/\\def/ || /\\newcommand/ || /\\renewcommand/ || /\\providecommand/)
    {
        ##JCC Ignore definitions:
        warn "Ignoring definition:\n  $_";
    }
    elsif (/\\include[{\s]+([^\001\040\011}]*)[\s}]/)
    {
      $full_filename = $1;
      $orig_filename = $full_filename;
      $full_filename = &find_file_ext($full_filename, 'tex', \@TEXINPUTS);
      if ($full_filename)
      {
      	push @includes,  $full_filename;
	if ( -e $full_filename )
	{
	  warn "	Found input file [$full_filename]\n";
	  &scan_for_includes_($full_filename);
	}
        else
        {
          if ( $orig_filename =~ /^\// )
          {
            warn "Latexmk: In \\include, ",
                 "could not find file [$orig_filename]\n";
          }
          else
          {
            warn "Latexmk: In \\include, ",
                 "could not find file [$orig_filename] in path [@TEXINPUTS]\n";
            warn "         assuming in current directory ($full_filename)\n";
          }
        }
      }
      else
      {
        if ( ! $force_include_mode )
        {
          if ( $orig_filename =~ /^\// )
          {
            die "Latexmk: In \\include, ",
                "could not find file [$orig_filename]\n";
          }
          else
          {
            die "Latexmk: In \\include, ",
                "could not find file [$orig_filename] in path [@TEXINPUTS]\n";
          }
        }
      }
    }
    elsif (/\\input[{\s]+([^\001\040\011}]*)[\s}]/)
    {
      $full_filename = $1;
      $orig_filename = $full_filename;
      $full_filename = &find_file_ext($full_filename, 'tex', \@TEXINPUTS);
      if ($full_filename)
      {
	push @includes, $full_filename;
#	warn "added '$full_filename'\n";
	if ( -e $full_filename )
	{
	  warn "	Found input for file [$full_filename]\n";
	  &scan_for_includes_($full_filename);
	}
	else
	{
	  if ( $orig_filename =~ /^\// )
	  {
	    warn "Latexmk: In \\input, could not find file [$orig_filename]\n";
	  }
	  else
	  {
	    warn "Latexmk: In \\input, ",
                 "could not find file [$orig_filename] in path [@TEXINPUTS]\n";
	    warn "         assuming in current directory ($full_filename)\n";
	  }
	}
      }
      else
      {
	if ( ! $force_include_mode )
	{
	  if ( $orig_filename =~ /^\// )
	  {
	    die "Latexmk: In \\input, could not find file [$orig_filename]\n";
	  }
	  else
	  {
	    die "Latexmk: In \\input, ",
                "could not find file [$orig_filename] in path [@TEXINPUTS]\n";
	  }
	}
      }
    }
    elsif (/\\blackandwhite{([^\001\040\011}]*)}/ || /\\colorslides{([^\001}]*)}/)
    {
############      $slide_mode = 1;
      $full_filename = &find_file_ext($1, 'tex', \@TEXINPUTS);
      if ($full_filename)
      {
      	push @includes, $full_filename;
	if ( -e $full_filename )
	{
	  warn "	Found slide input for file [$full_filename]\n";
	  &scan_for_includes_($full_filename);
	}
      }
    }
    elsif (/\\psfig{file=([^,}]+)/ || /\\psfig{figure=([^,}]+)/)
    {
      $orig_filename = $1;
      $full_filename = &find_file($1, \@psfigsearchpath);
      if ($full_filename)
      {
      	push @includes, $full_filename;
	if ( -e $full_filename )
	{
	  warn "	Found psfig for file [$full_filename]\n";
	}
      }
    }
    elsif ( /\\epsfbox{([^}]+)}/ || /\\epsfbox\[[^\]]*\]{([^}]+)}/ ||
	    /\\epsffile{([^}]+)}/ || /\\epsffile\[[^\]]*\]{([^}]+)}/ ||
	    /\\epsfig{file=([^,}]+)/ || /\\epsfig{figure=([^,}]+)/ )
    {
      $orig_filename = $1;
      $full_filename = &find_file($1, \@psfigsearchpath);
      if ($full_filename)
      {
      	push @includes, $full_filename;
	if ( -e $full_filename )
	{
	  warn "	Found epsf for file [$full_filename]\n";
	}
      }
    }
    elsif (
        /\\includegraphics{([^}]+)}/ || /\\includegraphics\[[^\]]*\]{([^}]+)}/
       )
    {
      $orig_filename = $1;
      $full_filename = &find_file_ext($1,'eps', \@psfigsearchpath);
      if ($full_filename)
      {
      	push @includes, $full_filename;
	if ( -e $full_filename )
	{
	  warn "	Found epsf for file [$full_filename]\n";
	}
      }
      else
      {
        warn "Latexmk: For \\includegraphics, ",
             "could not find file [$orig_filename]\n",
             "          in path [@psfigsearchpath]\n";
	if ( ! $force_include_mode ) {die "\n";}
      }
    }
    elsif (/\\documentstyle[^\000]+landscape/)
    {
      warn "	Detected landscape mode\n";
      $landscape_mode = 1;
    }
    elsif (/\\bibliography{([^}]+)}/)
    {
      @bib_files = split /,/, $1;
      &find_file_list1( \@bib_files, \@bib_files, '.bib', \@BIBINPUTS );
      warn "	Found bibliography files [@bib_files]\n" unless $silent;
      $bibtex_mode = 1;
    }
    elsif (/\\psfigsearchpath{([^}]+)}/)
    {
      @psfigsearchpath = &split_search_path(':', '', $1);
    }
    elsif (/\\graphicspath{([^}]+)}/)
    {
      @psfigsearchpath = &split_search_path(':', '', $1);
    }
    elsif (/\\makeindex/)
    {
      $index_mode = 1;
      warn "        Detected index mode\n";
    }
  }
}

#**************************************************
sub parse_log
{
# Scan log file for: include files, bibtex mode,
#    reference_changed, bad_reference, bad_citation
# In bibtex mode, scan aux file for bib files
# Return value: 1 if success, 0 if no log file.
# Set global variables:
#   @includes to list of included files that exist or that appear to be
#       genuine files (as opposed to incorrectly parsed names).
#   %includes_missing to list of files that latex appeared to search for
#        and didn't find, i.e., from error messages
#   @aux_files to list of .aux files.
#   Leave these unchanged if there is no log file.
#   $reference_changed, $bad_reference, $bad_citation


    my $log_name = "$root_filename.log";
    my $log_file = 0;
    if ( ! open( $log_file, "<$log_name" ) )
    {
        return 0;
    }
    my $line_number = 0;
    my $graphic_line = 0;
    my @bbl_files = ();
    my @ignored_input_files = ();
    my @existent = ();
    my @include_list = ($texfile_name);
    my @include_graphics_list = ();
    my %includes_from_errors = ();
    @includes = ();
    %includes_missing = ();


    $reference_changed = 0;
    $bad_reference = 0;
    $bad_citation = 0;

##  ?? New.  We'll determine these modes from parsing the file
    $bibtex_mode = 0;
    $index_mode = 0;

LINE:
   while(<$log_file>) {
      $line_number++;
      chomp;
      if ( $line_number == 1 ){
	  if ( /^This is / ) {
	      # First line OK\n";
              next LINE;
          } else {
             die "Error on first line of \"$log_name\".  This is not a TeX log file.\n$_";
	  }
      }
      # Handle wrapped lines:
      # They are lines brutally broken at exactly $log_wrap chars
      #    excluding line-end.
      my $len = length($_);
      while ($len == $log_wrap)
      {
        my $extra = <$log_file>;
        chomp $extra;
        $line_number++;
        $len = length($extra);
        $_ .= $extra;
      }
      # Check for changed references, bad references and bad citations:
      if (/Rerun to get/) {
          warn "\n=== References changed.\n";
          $reference_changed = 1;
      }
      if (/LaTeX Warning: (Reference[^\001]*undefined)./) {
	 warn "\n=== $1 \n";
         $bad_reference = 1;
      }
      if (/LaTeX Warning: (Citation[^\001]*undefined)./) {
	 warn "\n=== $1 \n";
         $bad_citation = 1;
      }
      if ( /^Document Class: / ) {
          # Latex message
	  next LINE;
      }
      if ( /^Output written on / ) {
          # Latex message
	  next LINE;
      }
      if ( /^Underfull / ) {
          # Latex error/warning
	  next LINE;
      }
      if ( /^Overfull / ) {
          # Latex error/warning
	  next LINE;
      }
      if ( /^\(Font\)/ ) {
	  # Font info line
          next LINE;
      }
      if ( /^Package: / ) {
          # Package sign-on line
	  next LINE;
      }
      if ( /^Document Class: / ) {
          # Class sign-on line
	  next LINE;
      }
      if ( /^Writing index file / ) {
          $index_mode =1;
          warn "Latexmk: Index file written, so turn on index_mode\n"
             unless $silent;
	  next LINE;
      }
      if ( /^No file .*?\.bbl./ ) {
          warn "Non-existent bbl file, so turn on bibtex_mode\n $_"
             unless $bibtex_mode == 1;
          $bibtex_mode = 1;
	  next LINE;
      }
      if ( /^File: ([^\s\[]*) Graphic file \(type / ) {
          # First line of message from includegraphics/x
          push @include_graphics_list, $1;
	  next LINE;
      }
      if ( /^File: / ) {
         # Package sign-on line. Includegraphics/x also produces a line
         # with this signature, but I've already handled it.
         next LINE;
      }
      if (/^\! LaTeX Error: File \`([^\']*)\' not found\./ ) {
	  $includes_missing{$1} = [3];
          next LINE;
      }
      if (/^\! LaTeX Error: / ) {
          next LINE;
      }
   INCLUDE_CANDIDATE:
       while ( /\((.*$)/ ) {
       # Filename found by
       # '(', then filename, then terminator.
       # Terminators: obvious candidates: ')':  end of reading file
       #                                  '(':  beginning of next file
       #                                  ' ':  space is an obvious separator
       #                                  ' [': start of page: latex
       #                                        and pdflatex put a
       #                                        space before the '['
       #                                  '[':  start of config file
       #                                        in pdflatex, after
       #                                        basefilename.
       #                                  '{':  some kind of grouping
       # Problem:
       #   All or almost all special characters are allowed in
       #   filenames under some OS, notably UNIX.  Luckily most cases
       #   are rare, if only because the special characters need
       #   escaping.  BUT 2 important cases are characters that are
       #   natural punctuation
       #   Under MSWin, spaces are common (e.g., "C:\Program Files")
       #   Under VAX/VMS, '[' delimits directory names.  This is
       #   tricky to handle.  But I think few users use this OS
       #   anymore.
       #
       # Solution: use ' [', but not '[' as first try at delimiter.
       # Then if candidate filename is of form 'name1[name2]', then
       #   try splitting it.  If 'name1' and/or 'name2' exists, put
       #   it/them in list, else just put 'name1[name2]' in list.
       # So form of filename is now:
       #  '(',
       # then any number of characters that are NOT ')', '(', or '{'
       #   (these form the filename);
       # then ' [', or ' (', or ')', or end-of-string.
       # That fails for pdflatex
       # In log file:
       #   '(' => start of reading of file, followed by filename
       #   ')' => end of reading of file
       #   '[' => start of page (normally preceeded by space)
       # Remember:
       #    filename (on VAX/VMS) may include '[' and ']' (directory
       #             separators)
       #    filenames (on MS-Win) commonly include space.

       # First step: replace $_ by whole of line after the '('
       #             Thus $_ is putative filename followed by other stuff.
          $_ = $1;
##          warn "==='$_'===\n";
          if ( /^([^\(^\)^\{]*?)\s\[/ ) {
              # Use *? in condition: to pick up first ' [' as terminator
              # 'file [' should give good filename.
          }
          elsif ( /^([^\(^\)^\{]*)\s(?=\()/ ) {
              # Terminator is ' (', but '(' isn't in matched string,
              # so we keep the '(' ready for the next match
          }
          elsif  ( /^([^\(^\)^\{]*)(\))/ ) {
              # Terminator is ')'
          }
	  else {
              #Terminator is end-of-string
	  }
##          warn "   ---'$1'---'$''---\n";
          $_ = $';       # Put $_ equal to the unmatched tail of string '
          my $include_candidate = $1;
          $include_candidate =~ s/\s*$//;   # Remove trailing space.
          if ( "$include_candidate" eq "[]" ) {
              # Part of overfull hbox message
              next INCLUDE_CANDIDATE;
          }
          # Put on list of new include files
          my @new_includes = ($include_candidate);
          if ( $include_candidate =~ /^(.+)\[([^\]]+)\]$/ ) {
             # Construct of form 'file1[file2]', as produced by pdflatex
             if ( -e $1 ) {
                 # If the first component exists, we probably have the
                 #   pdflatex form
                 @new_includes = ($1, $2);
	     }
             else {
                # We have something else.
                # So leave the original candidate in the list
	     }
	  }
	INCLUDE_NAME:
          foreach my $include_name (@new_includes) {
	      my ($base, $path, $ext) = fileparse ($include_name, '\.[^\.]*');
	      if ( $ext eq '.bbl' ) {
		  warn "Input bbl file \"$include_name\", so turn on bibtex_mode\n"
		     unless ($bibtex_mode == 1) || $silent;
		  $bibtex_mode = 1;
		  push @bbl_files, $include_name;
	      } elsif ( $ext eq ".aux" ) {
		  push @aux_files, $include_name;
		  push @ignored_input_files, $include_name;
	      } elsif ( $generated_exts{$ext} ) {
		  #warn "Ignoring '$include_name'\n";
		  push @ignored_input_files, $include_name;
	      } else {
		  push @include_list, $include_name;
	      }
	  } # INCLUDE_NAME
      } # INCLUDE_CANDIDATE
  }  # LINE
  close($log_file);
  @aux_files = &uniq( sort(@aux_files) );
  @ignored_input_files = &uniq( sort(@ignored_input_files) );

  if ( $bibtex_mode )
  {
      &parse_aux;
      push @include_list, @bib_files;
  }
  @include_list = &uniq(sort @include_list);
  @include_graphics_list = &uniq(sort @include_graphics_list);
  foreach (@include_list) {
      if ( -e $_ ) {
         push @existent, $_;
      } else {
         $includes_missing{$_} = [1];
      }
  }
  foreach (@include_graphics_list) {
      if ( -e $_ ) {
         push @existent, $_;
      } else {
         # I have to work harder finding the file
         $includes_missing{$_} = [2];
      }
  }

  my $non_exist = 0;
  my $not_found = 0;
  my $missing = 0;
  foreach (sort keys %includes_missing) {
      $missing++;
      my $code = ${$includes_missing{$_}}[0];
      if ($code == 1) {$non_exist ++;}
      if ($code == 2) {$not_found ++;}
  }


  @includes = @existent;

  if ( $diagnostics )
  {
     my $inc = $#include_list + 1;
     my $exist = $#existent + 1;
     my $non_exist = $#includes_missing + 1;
     my $bbl = $#bbl_files + 1;
     print "$inc included files detected, of which ";
     print "$exist exist, and $non_exist do not exist.\n";
     print "Input files that exist:\n";
     foreach (@existent) { print "   $_\n";}

     if ( $#bbl_files >= 0 ) {
        print "Input bbl files:\n";
        foreach (@bbl_files) { print "   $_\n";  }
     }
     if ( $#ignored_input_files >= 0 ) {
        print "Other input files that are generated via LaTeX run:\n";
        foreach (@ignored_input_files) { print "   $_\n";  }
     }
     if ( $missing > 0 ) {
        print "Apparent input files that appear NOT to exist:\n";
        print "  Some correspond to misunderstood lines in the .log file\n";
        print "  Some are files that latexmk failed to find\n";
        print "  Some really don't exist\n";
        foreach (sort keys %includes_missing) { print "   $_\n";  }
     }
  }
  return 1;
}

#************************************************************

sub parse_aux
# Parse aux_file for bib files.
# Return 3 with @bib_files set if aux_file exists, but I couldn't find all the bib_files
# Return 2 with @bib_files empty if aux_file exists, but there are no \bibdata
#   lines. In that case turn off bibtex mode, as side effect.
# Return 1 with @bib_files set if aux_file exists
# Return 0 and leave @bib_files unchanged in aux_file does not exist (or
#      cannot be opened)
{
   local($aux_file) = "$root_filename.aux";
   if (! open(aux_file) )
      { return 0; }
   @bib_files = ();
AUX_LINE:
   while (<aux_file>)
   {
      if ( /^\\bibdata\{(.*)\}/ )
      {
         # \\bibdata{comma_separated_list_of_bib_file_names}
         # (Without the '.bib' extension)
         push( @bib_files, split /,/, $1 );
      }
   }
   close(aux_file);
   if ( $#bib_files eq -1 ) {
       warn "No .bib files listed in .aux file, so turn off bibtex_mode\n";
       $bibtex_mode = 0;
       return 2;
   }
   my $bibret = &find_file_list1( \@bib_files, \@bib_files, '.bib', \@BIBINPUTS );
   if ($bibret == 0) {
      warn "Found bibliography files [@bib_files]\n" unless $silent;
   }
   else {
       warn "Failed to find one or more bibliography files in [@bib_files]\n";
       if ($force_mode) {
          warn "==== Force_mode is on, so I will continue.  But there may be problems ===\n";
       }
       else {
           $failure = -1;
           $failure_msg = 'Failed to find one or more bib files';
       }
       return 3;
   }
   return 1;
}

#************************************************************

sub update_depend_file
{
  warn "Writing dependency file [$root_filename.dep]\n";
  $rc_file = ">$root_filename.dep";
  open(rc_file) || die "Latexmk: Unable to open dependency file [$rc_file] for updating\n";
  print rc_file '@includes = (\n';
  my $first = 1;
  foreach my $name (@includes) {
      if (!$first) {print rc_file ",\n";}
      print rc_file "\'$name\'";
      $first = 0;
  }
  print rc_file "\n)\n";
  print rc_file '@bib_files = (\n';
  $first = 1;
  foreach $name (@bib_files) {
      if (!$first) {print rc_file ",\n";}
      print rc_file "\'$name\'";
      $first = 0;
  }
  print rc_file "\n)\n";
  if ($bibtex_mode)
  {
    print rc_file '$bibtex_mode = 1;' . "\n";
  }
  if ($index_mode)
  {
    print rc_file '$index_mode = 1;' . "\n";
  }
  print rc_file "\$view = \"$view\";\n";
  print rc_file "\$need_dvi = $need_dvi;\n";
  print rc_file "\$need_ps = $need_ps;\n";
  print rc_file "\$need_pdf = $need_pdf;\n";
  print rc_file "\$pdf_mode = $pdf_mode;\n";
  close rc_file;
}

#************************************************************
#************************************************************
#************************************************************
#
#      UTILITIES:
#

#************************************************************
# Miscellaneous

sub show_array {
# For use in debugging. $_[0] = label.  Rest of @_ is list of items
    print "$_[0]\n";
    shift;
    foreach (@_){ print "  $_\n";}
}

#************************************************************

sub glob_list {
    # Glob a collection of filenames.  Sort and eliminate duplicates
    # Usage: e.g., @globbed = glob_list(string, ...);
    my @globbed = ();
    foreach (@_) {
        push @globbed, glob;
    }
    return uniq( sort( @globbed ) );
}

#************************************************************
#      File handling routines:


#************************************************************

sub get_latest_mtime
# - arguments: each is a filename.
# - returns most recent modify time.
{
  my $return_mtime = 0;
  foreach my $include (@_)
  {
    my $include_mtime = &get_mtime($include);
    # The file $include may not exist.  If so ignore it, otherwise
    # we'll get an undefined variable warning.
    if ( ($include_mtime) && ($include_mtime >  $return_mtime) )
    {
      $return_mtime = $include_mtime;
    }
  }
  return $return_mtime;
}

#************************************************************
sub get_mtime
{
  my $mtime = (stat($_[0]))[9];
  return $mtime;
}

#************************************************************



# Find file with default extension
# Usage: find_file_ext( name, default_ext, ref_to_array_search_path)
sub find_file_ext
{
    my $full_filename = shift;
    my $ext = shift;
    my $ref_search_path = shift;
    my $full_filename1 = &find_file($full_filename, $ref_search_path, '1');
#print "Finding \"$full_filename\" with ext \"$ext\" ... ";
    if (( $full_filename1 eq '' ) || ( ! -e $full_filename1 ))
    {
      my $full_filename2 =
          &find_file("$full_filename.$ext",$ref_search_path,'1');
      if (( $full_filename2 ne '' ) && ( -e $full_filename2 ))
      {
        $full_filename = $full_filename2;
      }
      else
      {
        $full_filename = $full_filename1;
      }
    }
    else
    {
      $full_filename = $full_filename1;
    }
#print "Found \"$full_filename\".\n";
    return $full_filename;
}

#************************************************************
# given filename and path, return full name of file, or die if none found.
# when force_include_mode=1, only warn if an include file was not
# found, and return 0 (PvdS).
# Usage: find_file(name, ref_to_array_search_path, param)
sub find_file
{
  my $name = $_[0];
  my $ref_path = $_[1];
  my $dir;
  if ( $name =~ /^\// )
  {
    if ($force_include_mode)
    {
	if ( $_[2] eq '' )
	{
	  warn "Latexmk: Could not find file [$name]\n";
	}
	return("$name");
    }
    else
    {
	if (-e $name)
	{
	  return("$name");
	}
	die "Latexmk: Could not find file [$name]\n";
    }
  }
  foreach $dir ( @{$ref_path} )
  {
#warn "\"$dir\", \"$name\"\n";
    if (-e "$dir/$name")
    {
      return("$dir/$name");
    }
  }
  if ($force_include_mode)
  {
	if ( $_[2] eq '' )
	{
	  warn "Latexmk: Could not find file [$name] in path [@{$ref_path}]\n";
	  warn "         assuming in current directory (./$name)\n";
	}
	return("./$name");
  }
  else
  {
	if ( $_[2] ne '' )
	{
	  return('');
	}
# warn "\"$name\", \"$ref_path\", \"$dir\"\n";
  	die "Latexmk: Could not find file [$name] in path [@{$ref_path}]\n";
  }
}

#************************************************************
# Usage: find_file1(name, ref_to_array_search_path, param)
# Modified find_file, which doesn't die.
# Given filename and path, return array of:
#             full name
#             retcode
# On success: full_name = full name with path, retcode = 0
# On failure: full_name = given name, retcode = 1
# If second argument is absent, give warning
sub find_file1
{
  my $name = $_[0];
  my $ref_path = $_[1];
  my $action = $_[2];
  my $dir;
  if ( $name =~ /^\// )
  {
     # Absolute path
     if (-e $name)
        { return( $name, 0 );}
     else
        { return( $name, 1 );}
  }
  foreach $dir ( @{$ref_path} )
  {
#warn "\"$dir\", \"$name\"\n";
    if (-e "$dir/$name")
    {
      return("$dir/$name", 0);
    }
  }
  if ( $_[2] eq '' ) {
     warn "Latexmk: Could not find file [$name] in path [@{$ref_path}]\n";
  }
  return("$name" , 1);
}

#************************************************************

sub find_file_list1
# Modified version of find_file_list that doesn't die.
# Given output and input arrays of filenames, a file suffix, and a path,
# fill the output array with full filenames
# Return a status code:
# Retcode = 0 on success
# Retocde = 1 if at least one file was not found
# Usage: find_file_list1( ref_to_output_file_array,
#                         ref_to_input_file_array,
#                         suffix,
#                         ref_to_array_search_path
#                       )
{
  my $ref_output = $_[0];
  my $ref_input  = $_[1];
  my $suffix     = $_[2];
  my $ref_search = $_[3];

  my @return_list = ();    # Generate list in local array, since input
                           # and output arrays may be same
  my $retcode = 0;
  foreach my $file (@$ref_input)
  {
    my ($tmp_file, $find_retcode) = &find_file1( "$file$suffix", $ref_search );
    if ($tmp_file)
    {
    	push @return_list, $tmp_file;
    }
    if ( $find_retcode != 0 ) {
        $retcode = 1;
    }
  }
  @$ref_output = @return_list;
  return $retcode;
}

#************************************************************

sub find_dirs1 {
   # Same as find_dirs, but argument is single string with directories
   # separated by $search_path_separator
   find_dirs( &split_search_path( $search_path_separator, ".", $_[0] ) );
}


#************************************************************

sub find_dirs {
# @_ is list of directories
# return: same list of directories, except that for each directory
#         name ending in //, a list of all subdirectories (recursive)
#         is added to the list.
#   Non-existent directories and non-directories are removed from the list
#   Trailing "/"s are removed, and multiple "/" are collapsed to single "/".
    local @result = ();
    my $find_action
        = sub
          { ## Subroutine for use in File::find
            ## Check to see if we have a directory
	       if (-d) { push @result, $File::Find::name; }
	  };
    foreach my $directory (@_) {
        my $recurse = ( $directory =~ m[//$] );
        # Remove all trailing /s, since directory name with trailing /
        #   is not always allowed:
        $directory =~ s[/+$][];
        # Collapse multiple / to single /, since filenames with
        #   multiple slashes are not necessarily valid.
        $directory =~ s[/+][/];
	if ( ! -e $directory ){
            next;
	}
	elsif ( $recurse ){
            # Recursively search directory
            find( $find_action, $directory );
	}
        else {
            push @result, $directory;
	}
    }
    return @result;
}

#************************************************************

sub uniq
# Read arguments, delete neighboring items that are identical,
# return array of results
{
    my @sort = ();
    my ($current, $prev);
    my $first = 1;
    while (@_)
    {
	$current = shift;
        if ($first || ($current ne $prev) )
	{
            push @sort, $current;
            $prev = $current;
            $first = 0;
        }
    }
    return @sort;
}

#************************************************************

sub copy_file_and_time {
    # Copy file1 to file2, copying time
    # I think copy() already does this, but it may depend on version.
    my $source = shift;
    my $dest   = shift;
    my $retcode = copy ($source, $dest)
	and do {
              my $mtime = get_mtime($dest);
              utime $mtime, $mtime, $dest;
	  };
    return $retcode;
}

#************************************************************

sub copy_file_keep_time {
    # Copy file1 to file2, preserving time of file 2
    my $source = shift;
    my $dest   = shift;
    if (-e $dest) { return 1; }
    my $mtime = get_mtime($dest);
    my $retcode = copy ($source, $dest)
	and do {
              utime $mtime, $mtime, $dest;
	  };
    return $retcode;
}

#************************************************************

sub diff {
   # diff(filename1, filename2):
   #         Return 2 if either or both files cannot be opened.
   #                1 if the files are different
   #                0 if the files are the same
    my $file1 = new FileHandle;
    my $file2 = new FileHandle;
   # Note automatic close of files when they go out of scope.
    open ($file1, $_[0]) or return 2;
    open ($file2, $_[1]) or return 2;
    my $retcode = 0;
    while ( ( not eof($file1)) || ( not eof($file2) ) ){
	if ( <$file1> ne <$file2> ) {
            $retcode = 1;
            last;
        }
    }
    return $retcode;
}

#************************************************************

sub diff_OLDVERSION {
   # diff(filename1, filename2):
   #         Return 2 if either or both files cannot be opened.
   #                1 if the files are different
   #                0 if the files are the same
    local (*file1, *file2);
    unless(  open (file1, $_[0]) ) {
        return 2;
    }
    unless ( open (file2, $_[1])) {
	close (file1);
        return 2;
    }
    my $retcode = 0;
    while ( ( not eof(file1)) && ( not eof(file2) ) ){
	if ( <file1> ne <file2> ) {
            $retcode = 1;
            last;
        }
    }
    close (file1);
    close (file2);
    return $retcode;
}

#************************************************************

sub split_search_path
{
# Usage: &split_search_path( separator, default, string )
# Splits string by separator and returns array of the elements
# Allow empty last component.
# Replace empty terms by the default.
    my $separator = $_[0];
    my $default = $_[1];
    my $search_path = $_[2];
    my @list = split( /$separator/, $search_path);
    if ( $search_path =~ /$separator$/ ) {
        # If search path ends in a blank item, the split subroutine
	#    won't have picked it up.
        # So add it to the list by hand:
        push @list, "";
    }
    # Replace each blank argument (default) by current directory:
    for ($i = 0; $i <= $#list ; $i++ ) {
        if ($list[$i] eq "") {$list[$i] = $default;}
    }
    return @list;
}

#************************************************************
#************************************************************
#      Process/subprocess routines

sub Run {
# Usage: Run ("program arguments ");
#    or  Run ("start program arguments");
#    or  Run ("NONE program arguments");
# First form is just a call to system, and the routine returns after the
#    program has finished executing.
# Second form (with 'start') runs the program detached, as appropriate for
#    the operating system: It runs "program arguments &" on UNIX, and
#    "start program arguments" on WIN95 and WINNT.  If multiple start
#    words are at the beginning of the command, the extra ones are removed.
# Third form (with 'NONE') does not run anything, but prints an error
#    message.  This is provided to allow program names defined in the
#    configuration to flag themselves as unimplemented.
# Return value is a list (pid, exitcode):
#   If process is spawned sucessfully, and I know the PID,
#       return (pid, 0),
#   else if process is spawned sucessfully, but I do not know the PID,
#       return (0, 0),
#   else if process is run,
#       return (0, exitcode of process)
#   else (I fail to run the requested process)
#       return (0, suitable return code)
#   where return code is 1 if cmdline is null or begins with "NONE" (for
#                      an unimplemented command)
#                     or the return value of the system subroutine.


# Split command line into one word per element, separating words by
#    one (OR MORE) spaces:
    my @cmdline = split (/ +/, $_[0]);
    if (! @cmdline ) {
	warn "Latexmk::Run: Attempt to run a null program.";
        return (0, 1);
    }
    if ( "$cmdline[0]" eq "start" ) {
        # Run detached.  How to do this depends on the OS
        # But first remove extra starts (which may have been inserted
        # to force a command to be run detached, when the command
	# already contained a "start").
        while ($cmdline[0] eq "start") {shift (@cmdline);}
        return &Run_Detached (@cmdline);
    }
    elsif ( "$cmdline[0]" eq "NONE" ) {
        warn "Latexmk::Run: ",
             "Program not implemented for this version.  Command line:\n";
	warn "   \"@cmdline\"\n";
        return (0, 1);
    }
    else {
       # The command is given to system as a single argument, to force shell
       # metacharacters to be interpreted:
       return (0, system( join( ' ', @cmdline) ) );
   }
}

#************************************************************

sub Run_Detached {
# Usage: Run_Detached ("program arguments ");
# Runs program detached.  Returns 0 on success, 1 on failure.
# Under UNIX use a trick to avoid the program being killed when the
#    parent process, i.e., me, gets a ctrl/C, which is undesirable for pvc
#    mode.  (The simplest method, system ("program arguments &"), makes the
#    child process respond to the ctrl/C.)
# Return value is a list (pid, exitcode):
#   If process is spawned sucessfully, and I know the PID,
#       return (pid, 0),
#   else if process is spawned sucessfully, but I do not know the PID,
#       return (0, 0),
#   else if I fail to spawn a process
#       return (0, 1)



##    warn "Running \"@_\" detached...\n";

    if ( "$^O" eq "MSWin32" ){
        # Win95, WinNT, etc: Use MS's start command:
        return (0, system( "start", @_) );
    } else {
        # Assume anything else is UNIX or clone
        # For this purpose cygwin behaves like UNIX.
        ## warn "Run_Detached.UNIX: A\n";
        my $pid = fork();
        ## warn "Run_Detached.UNIX: B pid=$pid\n";
        if ( ! defined $pid ) {
            ## warn "Run_Detached.UNIX: C\n";
	    warn "Latexmk:: Could not fork to run the following command:\n";
            warn "   \"@_\"\n";
            return (0, 1);
	}
        elsif( $pid == 0 ){
           ## warn "Run_Detached.UNIX: D\n";
           # Forked child process arrives here
           # Insulate child process from interruption by ctrl/C to kill parent:
           #     setpgrp(0,0);
           # Perhaps this works if setpgrp doesn't exist
           #    (and therefore gives fatal error):
           eval{ setpgrp(0,0);};
           exec(@_);
           # Exec never returns; it replaces current process by new process
           die "Latexmk forked process: could not run the command\n",
               "  \"@_\"\n";
        }
        ##warn "Run_Detached.UNIX: E\n";
        # Original process arrives here
        return ($pid, 0);
    }
    # NEVER GET HERE.
    ##warn "Run_Detached.UNIX: F\n";
}

#************************************************************

sub find_process_id {
# find_process_id(string) finds id of process containing string and
# being run by the present user.  Typically the string will be the
# name of the process or part of its command line.
# On success, this subroutine returns the process ID.
# On failure, it returns 0.
# This subroutine only works on UNIX systems at the moment.

    if ( $pid_position < 0 ) {
        # I cannot do a ps on this system
        return (0);
    }

    my $looking_for = $_[0];
    my @ps_output = `$pscmd`;

    shift(@ps_output);  # Discard the header line from ps
    foreach (@ps_output)   {
	next unless ( /$looking_for/ ) ;
        my @ps_line = split (' ');
        return($ps_line[$pid_position]);
    }

    # No luck in finding the specified process.
    return(0);
}

#************************************************************


