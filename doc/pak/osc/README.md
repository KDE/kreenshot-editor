Use the openSUSE build service (OSC)
====================================

Note that with default settings osc uses space on the / partition when building and installing packages.
The default build root dirs are:
/var/tmp/build-root/
/var/tmp/build-root32/

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

FIRST..
    of all create a tar.gz snapshot of the current source and place it next to the spec file.
    Adapt the Source0 tag in the spec file if necessary.
    TODO: automate this.

E.g. build for openSUSE_13.1 and for the x86_64 platform:

    $ cd kreenshot-editor
    $ osc build openSUSE_13.1 x86_64     # provides only libqt5-qtbase-devel 5.1 but we need 5.2 or greater
    $ osc build openSUSE_Factory x86_64

This takes a while to download all the packages. Luckily they will be cached.
You will need the root password which is used to create some sandbox environment.
So your root directory will not be polluted.

Depending on the chosen configuration the build directory is located here:
/var/tmp/build-root/openSUSE_Factory-x86_64/home/abuild/rpmbuild/BUILD/kreenshot-editor-0.01/
