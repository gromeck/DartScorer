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


install-data-am:
	$(mkinstalldirs) $(DESTDIR)$(datarootdir)/$(PACKAGE)/voice
	@for DIR in *; do \
		if test -f $$DIR/game-on.wav ; then \
			echo "Installing voice $$DIR in $(DESTDIR)$(datarootdir)/$(PACKAGE)/voice/" ; \
			$(mkinstalldirs) $(DESTDIR)$(datarootdir)/$(PACKAGE)/voice/$$DIR ; \
			$(install_sh) -m 0644 $$DIR/* $(DESTDIR)$(datarootdir)/$(PACKAGE)/voice/$$DIR ; \
		fi ; \
	done
