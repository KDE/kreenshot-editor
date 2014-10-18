# This file is part of the kreenshot-editor project.
#
# Copyright (C) 2014 by Gregor Mi <codestruct@posteo.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 or the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
#

#
# spec file for package kreenshot-editor
#

# Date        Author           Changelog
# ----        ------           ---------
# 2014-10-18  Gregor Mi        init
# TODO:
#   add screenshot to http://software.opensuse.org/
#   specify Requires tag?

# The Name must match the openSUSE build service package name
# (see for example https://build.opensuse.org/package/show/home:codeminister/FreeFileSync)
# in order to get a properly filled download page
#
Name:           kreenshot-editor
Summary:        Screenshot image editing
Version:        0.01
Release:        0
License:        LGPL-2.0+
Group:          Productivity/Graphics
Url:            http://kreenshot.wordpress.com
Source0:        kreenshot-editor-aff3428.tar.gz
#Source1:        ChangeLog
#Patch0:         0001-progress_indicator.cpp-fix-by-using-wxString-ctor.patch
#
# NOTE that the percent sign in comments should be quoted by another percent sign
#
# see https://en.opensuse.org/openSUSE:Specfile_guidelines
# "The BuildRoot tag should always be used, even if newer rpms override it anyway. The preferred path is %%{_tmppath}/%%{name}-%%{version}-build."
# TODO: use this instead of just build
#BuildRoot:      %%{_tmppath}/%%{name}-%%{version}-build
BuildRoot:      build
BuildRequires:  unzip
BuildRequires:  gcc-c++ >= 4.8
BuildRequires:  cmake >= 2.8.11
# openSUSE_13.1 comes with version 5.1.1
# openSUSE_Factory                 5.3.x
BuildRequires:  libqt5-qtbase-devel >= 5.2
#PreReq:         %%fillup_prereq


%description
kreenshot-editor is an application for screenshot image editing.
Still INCUBATING, more info here: http://kreenshot.wordpress.com
Features:
* draw basic shapes like rectangle, ellipse, line, arrow etc.
* highlight, obfuscate or crop rectangular regions
* undo/redo
* command line interface
* import from clipboard
* default output directory with configurable filename pattern


%prep
%setup -q -c %{name}-%{version}
%define _use_internal_dependency_generator 0

# patches http://www.redhat.com/support/resources/howto/RH-sendmail-HOWTO/x192.html
# https://fedoraproject.org/wiki/How_to_create_an_RPM_package
#%%patch0 -p1

####???
####%%{__cp} %%{S:1} .


%build
# see https://en.opensuse.org/openSUSE:Specfile_guidelines
#    NOTE that the %%{buildroot} is cleaned automatically
#    because %%clean is called interally
mkdir build
# so directory structure now looks like this:
#  build (new)
#  doc
#  src
#  tests
cd build
# see also http://www.cmake.org/Wiki/CMake_Useful_Variables for information about CMAKE_BUILD_TYPE
# we need to set CMAKE_INSTALL_PREFIX because otherwise it installs to /usr/local
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..
make


%install
cd build
# see https://en.opensuse.org/openSUSE:Specfile_guidelines
#     "%%make_install is a macro available starting rpm-4.10. It is equivalent to `make install DESTDIR="%%{?buildroot}"`."
%make_install


%clean
# see "Removing the buildroot" on https://en.opensuse.org/openSUSE:Specfile_guidelines
rm -rf %{buildroot}


# to avoid "error: Installed (but unpackaged) file(s) found"
%files
# see http://www.rpm.org/max-rpm-snapshot/s1-rpm-inside-files-list-directives.html
# %%defattr(<The default permissions, or "mode" for files>,
#            <The default user id>,
#            <The default group id>,
#            <The default permissions, or "mode" for directories.>)
# %%defattr(-,root,root,-) # causes warning in combination with standard dirs /usr/lib64, /usr/bin, /usr/share

# warning standard-dir-owned-by-package /usr/bin
# solved by:
%_bindir
# see also http://docs.fedoraproject.org/en-US/Fedora_Draft_Documentation/0.1/html/RPM_Guide/ch09s07.html (Built-in macros)

# warning standard-dir-owned-by-package /usr/lib64
# solved by:
%_libdir

# /usr/share/kreenshot-editor/
%_datadir

#%%doc /usr/share/doc/kreenshot-editor
##%%doc /usr/share/doc/kreenshot-editor/changelog.gz
%doc COPYING.LIB


%changelog
# ... TODO

# [   89s] RPMLINT report:
# [   89s] ===============
# [   91s] kreenshot-editor.x86_64: W: standard-dir-owned-by-package /usr/lib64
# [   91s] kreenshot-editor.x86_64: W: standard-dir-owned-by-package /usr/bin
# [   91s] kreenshot-editor.x86_64: W: standard-dir-owned-by-package /usr/share
# [   91s] This package owns a directory that is part of the standard hierarchy, which
# [   91s] can lead to default directory permissions or ownerships being changed to
# [   91s] something non-standard.
# [   91s]
# [   91s] kreenshot-editor.x86_64: W: no-manual-page-for-binary kreenshot-editor
# [   91s] Each executable in standard binary directories should have a man page.
# [   91s]
# [   91s] kreenshot-editor.x86_64: W: no-changelogname-tag
# [   91s] kreenshot-editor.src: W: no-changelogname-tag
# [   91s] There is no changelog. Please insert a '%changelog' section heading in your
# [   91s] spec file and prepare your changes file using e.g. the 'osc vc' command.
# [   91s]
# [   91s] kreenshot-editor.src:153: W: macro-in-comment %build)
# [   91s] kreenshot-editor.src:157: W: macro-in-comment %build)
# [   91s] There is a unescaped macro after a shell style comment in the specfile. Macros
# [   91s] are expanded everywhere, so check if it can cause a problem in this case and
# [   91s] escape the macro with another leading % if appropriate.
# [   91s]
# [   91s] kreenshot-editor.src:153: W: macro-in-%changelog %build)
# [   91s] kreenshot-editor.src:157: W: macro-in-%changelog %build)
# [   91s] Macros are expanded in %changelog too, which can in unfortunate cases lead to
# [   91s] the package not building at all, or other subtle unexpected conditions that
# [   91s] affect the build.  Even when that doesn't happen, the expansion results in
# [   91s] possibly "rewriting history" on subsequent package revisions and generally odd
# [   91s] entries eg. in source rpms, which is rarely wanted.  Avoid use of macros in
# [   91s] %changelog altogether, or use two '%'s to escape them, like '%%foo'.
# [   91s]
# [   91s] kreenshot-editor.src:128: W: files-attr-not-set
# [   91s] kreenshot-editor.src:133: W: files-attr-not-set
# [   91s] kreenshot-editor.src:136: W: files-attr-not-set
# [   91s] kreenshot-editor.src:140: W: files-attr-not-set
# [   91s] A file or a directory entry in a %files section does not have attributes set
# [   91s] which may result in unexpected file permissions and thus security issues in
# [   91s] the resulting binary package depending on the build environment and rpmbuild
# [   91s] version (typically < 4.4).  Add default attributes using %defattr before it in
# [   91s] the %files section, or use per entry %attr's.
# [   91s]
# [   91s] 2 packages and 0 specfiles checked; 0 errors, 14 warnings.
# [   91s]
# [   91s]
# [   91s] catgroove.swingsite finished "build kreenshot-editor.spec" at Sat Oct 18 15:27:05 UTC 2014.
# [   91s]
