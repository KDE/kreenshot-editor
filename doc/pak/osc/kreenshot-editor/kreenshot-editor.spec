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
Name:           kreenshot-editor
Summary:        Screenshot image editor
Version:        0.01
Release:        0
License:        LGPL-2.0+
Group:          Productivity/Graphics
Url:            http://kreenshot.wordpress.com
Source0:        kreenshot-editor-9b7793f.tar.gz
#Source1:        ChangeLog
#Patch0:         0001-progress_indicator.cpp-fix-by-using-wxString-ctor.patch
#BuildRoot:      %{_tmppath}/%{name}-%{version}-build
BuildRoot:      build
BuildRequires:  unzip
BuildRequires:  gcc-c++ >= 4.8
BuildRequires:  cmake >= 2.8.11
BuildRequires:  libqt5-qtbase-devel >= 5.2
#BuildRequires:  boost-devel >= 1.53
#BuildRequires:  wxWidgets-2_9-devel
#BuildRequires:  wxWidgets-devel
#BuildRequires:  wxWidgets-wxcontainer-devel
#BuildRequires:  wxWidgets-2_9-wxcontainer-devel
#PreReq:         %fillup_prereq

%description
abc
def
ghi

%prep
%setup -q -c %{name}-%{version}
%define _use_internal_dependency_generator 0

# patches http://www.redhat.com/support/resources/howto/RH-sendmail-HOWTO/x192.html
# https://fedoraproject.org/wiki/How_to_create_an_RPM_package
#%patch0 -p1

####???
####%{__cp} %{S:1} .

%build
mkdir build
cd build
cmake ..
make

%install
# cd FreeFileSync/Source
# %make_install

%clean
rm -rf %{buildroot}

%files
#%defattr(-,root,root,-)
#%{_bindir}/abc
#%{_datadir}/def
#%doc /usr/share/doc/kreenshot-editor
##%doc /usr/share/doc/kreenshot-editor/changelog.gz
%doc COPYING.LIB

%changelog
