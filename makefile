CPPFLAGS += -pedantic-errors -std=c++11

all: main

# action phase object files ends with f
# class/struct object files ends with c

main: objectFiles/main.o objectFiles/gamec.o objectFiles/io.o objectFiles/menu.o objectFiles/play.o objectFiles/level.o objectFiles/setting.o objectFiles/statf.o objectFiles/settingf.o objectFiles/menuWrapperc.o objectFiles/color.o objectFiles/levelf.o objectFiles/infantryc.o objectFiles/gamePrintStatusc.o objectFiles/gamePrintBuildc.o objectFiles/gameBuildf.o objectFiles/gameBuildSubf.o objectFiles/gameRemoveBuildSubf.o objectFiles/gamePrintResearchf.o objectFiles/gameResearchf.o objectFiles/gameResearchSubf.o objectFiles/gameUpgradeSub.o
	g++ -o $@ $^ -pthread

main.exe: objectFiles/main.o objectFiles/gamec.o objectFiles/io.o objectFiles/menu.o objectFiles/play.o objectFiles/level.o objectFiles/setting.o objectFiles/statf.o objectFiles/settingf.o objectFiles/menuWrapperc.o objectFiles/color.o objectFiles/levelf.o objectFiles/infantryc.o objectFiles/gamePrintStatusc.o objectFiles/gamePrintBuildc.o objectFiles/gameBuildf.o objectFiles/gameBuildSubf.o objectFiles/gameRemoveBuildSubf.o objectFiles/gamePrintResearchf.o objectFiles/gameResearchf.o objectFiles/gameResearchSubf.o objectFiles/gameUpgradeSub.o
	x86_64-w64-mingw32-gcc -o $@ $^ -pthread

objectFiles/main.o: main.cpp class/game/game.h menu/menu.h io/io.h action/stat/stat.h action/setting/setting.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gamec.o: class/game/game.cpp class/game/game.h class/game/gameStruct.h class/game/gameUnit.h lib/uuid/uuid.hpp
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/io.o: io/io.cpp io/io.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/menu.o: menu/menu.cpp menu/menu.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/play.o: menu/play.cpp menu/menu.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/level.o: menu/level.cpp menu/menu.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/setting.o: menu/setting.cpp menu/menu.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/statf.o: action/stat/stat.cpp action/stat/stat.h io/io.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/settingf.o: action/setting/setting.cpp action/setting/setting.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/menuWrapperc.o: class/menuWrapper/menuWrapper.cpp class/menuWrapper/menuWrapper.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/color.o: io/color.cpp io/io.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/levelf.o: action/level/level.cpp action/level/level.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/infantryc.o: data/troop/infantry.cpp data/troop/troop.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gamePrintStatusc.o: class/game/gamePrintStatus.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gamePrintBuildc.o: class/game/gamePrintBuild.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameBuildf.o: class/game/actionClass/gameBuild.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameBuildSubf.o: class/game/actionClass/gameBuildSub.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameRemoveBuildSubf.o: class/game/actionClass/gameRemoveBuildSub.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gamePrintResearchf.o: class/game/gamePrintResearch.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameResearchf.o: class/game/actionClass/gameResearch.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameResearchSubf.o: class/game/actionClass/gameResearchSub.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameUpgradeSub.o: class/game/actionClass/gameUpgradeSub.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

clean:
	rm objectFiles/*.o
	rm main
	
PHONY: clean