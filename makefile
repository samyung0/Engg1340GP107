CPPFLAGS += -g -pedantic-errors -std=c++11

all: main

# action phase object files ends with f
# class/struct object files ends with c

main: objectFiles/main.o objectFiles/gamec.o objectFiles/io.o objectFiles/menu.o objectFiles/play.o objectFiles/level.o objectFiles/setting.o objectFiles/statf.o objectFiles/settingf.o objectFiles/menuWrapperc.o objectFiles/color.o objectFiles/levelf.o objectFiles/gamePrintStatusc.o objectFiles/gamePrintBuildc.o objectFiles/gameBuildf.o objectFiles/gameBuildSubf.o objectFiles/gameRemoveBuildSubf.o objectFiles/gamePrintResearchf.o objectFiles/gameResearchf.o objectFiles/gameResearchSubf.o objectFiles/gameUpgradeSubf.o objectFiles/infantryc.o objectFiles/calvaryc.o objectFiles/suicideBomberc.o objectFiles/artilleryc.o objectFiles/logisticc.o objectFiles/armoredCarc.o objectFiles/tank1c.o objectFiles/tank2c.o objectFiles/tankOshimaic.o objectFiles/casc.o objectFiles/fighterc.o objectFiles/bomberc.o objectFiles/kamikazec.o objectFiles/troopc.o objectFiles/gamePrintTroopc.o objectFiles/gameTrainf.o objectFiles/gameTrainSubf.o objectFiles/gameRemoveSubf.o objectFiles/armyUnitc.o objectFiles/gameArmyf.o objectFiles/sensouf.o objectFiles/randomf.o objectFiles/savef.o objectFiles/loadf.o
	g++ -g -o $@ $^ -pthread

objectFiles/main.o: main.cpp class/game/game.h menu/menu.h io/io.h action/stat/stat.h action/setting/setting.h action/load/load.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gamec.o: class/game/game.cpp class/game/game.h class/game/gameStruct.h lib/uuid/uuid.hpp
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
 
objectFiles/gamePrintStatusc.o: class/game/gamePrintStatus.cpp class/game/game.h io/io.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gamePrintBuildc.o: class/game/gamePrintBuild.cpp class/game/game.h io/io.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameBuildf.o: class/game/actionClass/gameBuild.cpp class/game/game.h class/game/gameStruct.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameBuildSubf.o: class/game/actionClass/gameBuildSub.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameRemoveBuildSubf.o: class/game/actionClass/gameRemoveBuildSub.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gamePrintResearchf.o: class/game/gamePrintResearch.cpp class/game/game.h io/io.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameResearchf.o: class/game/actionClass/gameResearch.cpp class/game/game.h class/game/gameStruct.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameResearchSubf.o: class/game/actionClass/gameResearchSub.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameUpgradeSubf.o: class/game/actionClass/gameUpgradeSub.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/infantryc.o: data/troop/infantry.cpp data/troop/troop.h class/damage/damage.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/calvaryc.o: data/troop/calvary.cpp data/troop/troop.h class/damage/damage.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/suicideBomberc.o: data/troop/suicideBomber.cpp data/troop/troop.h class/damage/damage.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/artilleryc.o: data/troop/artillery.cpp data/troop/troop.h class/damage/damage.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/logisticc.o: data/troop/logistic.cpp data/troop/troop.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/armoredCarc.o: data/troop/armoredCar.cpp data/troop/troop.h class/damage/damage.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/tank1c.o: data/troop/tank1.cpp data/troop/troop.h class/damage/damage.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/tank2c.o: data/troop/tank2.cpp data/troop/troop.h class/damage/damage.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/tankOshimaic.o: data/troop/tankOshimai.cpp data/troop/troop.h 
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/casc.o: data/troop/cas.cpp data/troop/troop.h class/damage/damage.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/fighterc.o: data/troop/fighter.cpp data/troop/troop.h class/damage/damage.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/bomberc.o: data/troop/bomber.cpp data/troop/troop.h class/damage/damage.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/kamikazec.o: data/troop/kamikaze.cpp data/troop/troop.h class/damage/damage.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/troopc.o: data/troop/troop.cpp data/troop/troop.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gamePrintTroopc.o: class/game/gamePrintTroop.cpp class/game/game.h io/io.h data/troop/troop.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameTrainf.o: class/game/actionClass/gameTrain.cpp class/game/game.h class/game/gameStruct.h data/troop/troop.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameTrainSubf.o: class/game/actionClass/gameTrainSub.cpp class/game/game.h data/troop/troop.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameRemoveSubf.o: class/game/actionClass/gameRemoveTroopSub.cpp class/game/game.h data/troop/troop.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/armyUnitc.o: class/game/gameUnit/armyUnit.cpp class/game/gameStruct.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/gameArmyf.o: class/game/actionClass/gameArmy.cpp class/game/game.h io/io.h class/game/gameStruct.h data/troop/troop.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/sensouf.o: class/game/sensou.cpp class/game/game.h io/io.h class/game/gameStruct.h data/troop/troop.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/randomf.o: random/random.cpp random/random.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/savef.o: class/game/actionClass/gameSave.cpp class/game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/loadf.o: action/load/load.cpp action/load/load.h
	g++ ${CPPFLAGS} -c $< -o $@

clean:
	rm objectFiles/*.o
	rm main
	
PHONY: clean