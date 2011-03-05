Name: stechec
Summary: Game's controller based on turn by turn
Version: 3.1
Release: 1
License: GPL
URL: http://www.prologin.org/
Group: Amusements/Games
Source: %{name}-%{version}.tar.bz2
BuildRoot: /var/tmp/%{name}-%{version}-%{release}-buildroot
Requires: ruby 

%description
stechec is the program used by the Prologin association to run
player's source code. Given a specific subject (usually a map-based
game, where you must move your units, collect resources and kill
others players), a player must program his own AI, and compete with
other. About 5 languages (C/C++/Pascal/Ocaml/Java) are
available to the player.

%prep
%setup -q

%build
./configure \
  --prefix=/usr \
  --sysconfdir=/etc \
  --mandir=/usr/share/man \
  --with-modules=ant,prolo2005,prolo2006
make

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%config /etc/stechec.yml
/usr/bin/*
/usr/lib/*
/usr/share/stechec/*
%doc /usr/share/man/*
%doc README README.stechec doc/*.pdf

%changelog
* Wed Aug  2 2006 Olivier Gournet <victor@le-poulpe.net>
- retrieve stechec rpm spec.
* Mon Jun 27 2005 Olivier Gournet <victor@lrde.epita.fr> 
- first rpm integration
