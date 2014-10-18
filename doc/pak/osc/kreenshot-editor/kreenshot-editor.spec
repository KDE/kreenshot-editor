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
#   * add screenshot to http://software.opensuse.org/
#   * specify Requires tag for runtime?

# The Name must match the openSUSE build service package name
# (see for example https://build.opensuse.org/package/show/home:codeminister/FreeFileSync)
# in order to get a properly filled download page
#
Name:           kreenshot-editor
Summary:        Screenshot image editing
Version:        0.03
Release:        0
License:        LGPL-2.0+
Group:          Productivity/Graphics
Url:            http://kreenshot.wordpress.com
Source0:        kreenshot-editor-%{version}.tar.gz
#Source1:        ChangeLog
#Patch0:         0001-progress_indicator.cpp-fix-by-using-wxString-ctor.patch
#
# NOTE that the percent sign in comments should be quoted by another percent sign
#
# see https://en.opensuse.org/openSUSE:Specfile_guidelines
# "The BuildRoot tag should always be used, even if newer rpms override it anyway. The preferred path is %%{_tmppath}/%%{name}-%%{version}-build."
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
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

# `pwd`, e.g. /var/tmp/build-root/openSUSE_Factory-x86_64/home/abuild/rpmbuild/BUILD/kreenshot-editor-0.02/
# contains the unpacked sources
# so directory structure now looks like this:
#  doc/
#  src/
#  tests/
#  CMakeLists.txt
#  COPYING
#  ...

# BUILDDIR=`pwd`
# %%{buildroot}: e.g. /var/tmp/build-root/openSUSE_Factory-x86_64/home/abuild/rpmbuild/BUILDROOT/kreenshot-editor-0.02-0.x86_64/
# does not work, causes ERROR:
#      RPATH "/home/abuild/rpmbuild/BUILDROOT/kreenshot-editor-0.02-0.x86_64/src/lib/kreenshoteditor" on /home/abuild/rpmbuild/BUILDROOT/kreenshot-editor-0.02-0.x86_64/tests/TestCore is not allowed
# later when doing the %%files section
# cd %%{buildroot}

# e.g. /var/tmp/build-root/openSUSE_Factory-x86_64/home/abuild/rpmbuild/BUILD/kreenshot-editor-0.02/build/
mkdir build
cd build

# To see the current list of macros, put a %%dump at the start of your spec file.
# see http://docs.fedoraproject.org/en-US/Fedora_Draft_Documentation/0.1/html/RPM_Guide/ch09s07s02.html
# WORKAROUND (see LIB_SUFFIX in CMakeLists.txt, how to do it more nicely?)
export KREEN_SPEC_ARCH=%{_arch}
echo KREEN_SPEC_ARCH=$MY_SPEC_ARCH

# see also http://www.cmake.org/Wiki/CMake_Useful_Variables for information about CMAKE_BUILD_TYPE
# we need to set CMAKE_INSTALL_PREFIX because otherwise it installs to /usr/local
# cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr $BUILDDIR # does not work, see above
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..

make


%install
# cd %%{buildroot} # does not work, see strange RPATH error above
cd build
# see https://en.opensuse.org/openSUSE:Specfile_guidelines
#     "%%make_install is a macro available starting rpm-4.10. It is equivalent to `make install DESTDIR="%%{buildroot}"`."
%make_install
# installs to /home/abuild/rpmbuild/BUILDROOT/kreenshot-editor-0.03-0.x86_64/usr/

# potential error in %%files when installing manpage:
# File not found: /home/abuild/rpmbuild/BUILDROOT/kreenshot-editor-0.03-0.x86_64/usr/share/man/man1/kreenshot-editor.1.gz
# quick fix, todo: looks not so nice,
#   if causes errors later, just remove the man page stuff and live with the RPMLINT warning that there is no man page:
cd ..
gzip %{name}.1
TARGETMANFILE=%{name}.1%{ext_man}
mkdir --parents %{buildroot}%{_mandir}/man1
cp $TARGETMANFILE %{buildroot}%{_mandir}/man1/$TARGETMANFILE


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
# %%defattr(-,root,root,-) # example
# values taken from https://build.opensuse.org/package/view_file/KDE:Qt5/libqt5-qtbase/libqt5-qtbase.spec
%defattr(-,root,root,755)
# fixes warnings:
#   kreenshot-editor.src:125: W: files-attr-not-set

# warning standard-dir-owned-by-package /usr/bin
# solved by adding specific filename (name expands to kreenshot-editor which is set above with Tag Name):
%_bindir/%{name}
# see also http://docs.fedoraproject.org/en-US/Fedora_Draft_Documentation/0.1/html/RPM_Guide/ch09s07.html (Built-in macros)
# and https://en.opensuse.org/openSUSE:Packaging_checks#standard-dir-owned-by-package

# note: without dash
%_libdir/libkreenshoteditor.so

# /usr/share/kreenshot-editor/
%_datadir/%{name}

# goes to /usr/share/doc/packages/kreenshot-editor/
%doc AUTHORS COPYING README ChangeLog

# %%doc part taken from https://build.opensuse.org/package/view_file/Education/xplanet/xplanet.spec
# but this causes some hassle, see %%install section
# see also http://www.cyberciti.biz/faq/linux-unix-creating-a-manpage/
%doc %{_mandir}/man1/%{name}.1%{ext_man}


# kreenshot-editor.changes
%changelog
