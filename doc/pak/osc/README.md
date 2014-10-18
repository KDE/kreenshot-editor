Use the openSUSE build service (OSC)
====================================

One time init (already done)
----------------------------
1.  Create package at build.opensuse.org via Web frontend
    https://build.opensuse.org/package/show/home:codeminister/kreenshot-editor

2.  Note that the package name here and in the spec file must match to make the package appear
    on http://software.opensuse.org/

3.  # CHECKOUT:

    $ cd doc/pak/osc
    $ osc co home:codeminister kreenshot-editor --current-dir

    # the subdirectory kreenshot-editor will be created.

4.  # INITAL COMMIT:

    $ cd kreenshot-editor
    $ osc add *
    A    kreenshot-editor-9b7793f.tar.gz
    A    kreenshot-editor.spec

    $ osc commit
    # Enter interactive commit message
    Sending    kreenshot-editor-9b7793f.tar.gz
    Sending    kreenshot-editor.spec
    Transmitting file data ..
    Committed revision 1.

Pushing changes
---------------
COMMIT A LOCAL CHANGE:
    $ cd kreenshot-editor
Change something.
    $ osc commit --message="minor fix"
Sending    kreenshot-editor.spec
Transmitting file data .
Committed revision 2.

In case someone changed something on the remote side:
UPDATE:
    $ cd kreenshot-editor
    $ osc update


Build locally using osc
-----------------------
Building locally means check if spec file is ready to be published.

E.g. build for openSUSE_13.1 and for the x86_64 platform:

    $ cd kreenshot-editor
    $ osc build openSUSE_13.1 x86_64
