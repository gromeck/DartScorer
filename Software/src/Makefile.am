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

bin_PROGRAMS=DartScorer

DartScorer_SOURCES=\
		main.cpp \
		camera.cpp \
		Config.cpp \
		Database.cpp \
		Points.cpp \
		PointsQueue.cpp \
		Player.cpp \
		DartScorer.cpp \
		Turn.cpp \
		Board.cpp \
		GameUI.cpp \
		ScoreHistoryWidget.cpp \
		ScoreInputWidget.cpp \
		GameX01Widget.cpp \
		PlayerManagementUI.cpp \
		PlayerManagementEditUI.cpp \
		ScoreInputUI.cpp \
		Statistics.cpp \
		StatisticsWidget.cpp \
		Voice.cpp \
		BoardUI.cpp \
		BoardWidget.cpp \
		DartScorerGUI.cpp

main.cpp: DartScorerGUI.h

DartScorerGUI.cpp DartScorerGUI.h: DartScorerGUI.fl
	fluid -c DartScorerGUI.fl

CLEANFILES =  DartScorerGUI.cpp DartScorerGUI.h

AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -DDATADIR=\"$(datarootdir)/$(PACKAGE)/\"
DartScorer_LDADD= @LIBINTL@

install-exec-am:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/games
	$(install_sh) $(bin_PROGRAMS) $(DESTDIR)$(prefix)/games
