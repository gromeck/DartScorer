#
#
#   DartScorer
#
#   (c) 2023 by Christian.Lorenz@gromeck.de
#
#   This file is part of DartScorer
#
#   DartScorer is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   DartScorer is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with DartScorer.  If not, see <https://www.gnu.org/licenses/>.
#
#

SUBDIRS= src po images voices etc

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = m4/ChangeLog

%.6.gz: %.6
	gzip <$< >$@

install-data-am: DartScorer.6.gz
	$(mkinstalldirs) $(DESTDIR)$(datarootdir)/man/man6
	$(install_sh) -m 0644 $< $(DESTDIR)$(datarootdir)/man/man6/
