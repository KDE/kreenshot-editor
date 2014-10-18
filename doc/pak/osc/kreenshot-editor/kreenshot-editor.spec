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
#

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
Source0:        kreenshot-editor-9b7793f.tar.gz
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
#BuildRequires:  boost-devel >= 1.53
#BuildRequires:  wxWidgets-2_9-devel
#BuildRequires:  wxWidgets-devel
#BuildRequires:  wxWidgets-wxcontainer-devel
#BuildRequires:  wxWidgets-2_9-wxcontainer-devel
#PreReq:         %%fillup_prereq

%description
kreenshot-editor is an application for screenshot image editing.
Features:
* draw basic shapes like rectangle, ellipse, line, arrow etc.
* highlight, obfuscate or crop rectangular regions
* all items are movable and resizable after they have been placed
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
cmake -DCMAKE_BUILD_TYPE=Release ..
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
rm -rf %{buildroot}
%files
#%%defattr(-,root,root,-)
#%%{_bindir}/abc
#%%{_datadir}/def
#%%doc /usr/share/doc/kreenshot-editor
##%%doc /usr/share/doc/kreenshot-editor/changelog.gz
%doc COPYING.LIB

%changelog
