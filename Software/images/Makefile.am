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

IMAGESDIR=$(DESTDIR)$(datarootdir)/$(PACKAGE)/images

install-data-am:
	$(mkinstalldirs) $(IMAGESDIR)
	$(install_sh) -m 0644 background/background.jpg $(IMAGESDIR)
	$(install_sh) -m 0644 logo/DartScorer-Logo-black.svg $(IMAGESDIR)/logo-black.svg
	$(install_sh) -m 0644 logo/DartScorer-Logo-white.svg $(IMAGESDIR)/logo-white.svg
	$(install_sh) -m 0644 logo/DartScorer-Logo-Icon.svg $(IMAGESDIR)/icon.svg
