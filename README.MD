# Hearts of Gold

Welcome to a text-based replica of the Hearts of Iron, one of the most intricate game of the century, where you build and devour other countries.

## Content
- [Goal of level](#a1)
- [Progression of game](#a2)
- __[Map design and coordinate system](#map) (must read)__
- [Techinal details](#a3)
    - [Building](#building)
    - [Researching](#research)
    - [Troops training](#troops)
    - [Army Editing](#army)
    - [Battle planning](#plan)
    - [Battling](#battle)
- [Advice for beginner](#a4)
- [Other details and tips](#a5)
- [All commands](#command)
- [Custom levels](#a6)
- [Implementation] (to be worked on)
## <a id="a1">Goal</a>

Your objective is to capture all countries in the map __within a time limit__. A country will capitulate once 50% of its land are captured.

___1 second in real life equals 1 day in the game. Each level will have a different time limit.___

## <a name="a2"></a>Progression of game

You can perform the following actions anytime during the game:

1. [Building](#building)
2. [Researching](#research)
3. [Troops training](#troops)
4. [Army Editing](#army)
5. [Battle planning](#plan)
6. [Battling](#battle)
  
Simply put, getting more buildings will boost your resources available for building more buildings and training troops.

Researching does not take any resources except time which varies from different types, some may give attack bonus or increased farm production just to name a few.

To prepare attacking, you must first train some troops. You may group them into different armies to make it more organized.

You may also, but not required, to plan the battle line, e.g. immediately attack another connected piece of land after the current one so no reinforcement will arrive so quickly.

The outcome of the battle depends on the types and amount of troops you and the enemy have, the attack bonuses, the air support and the terrain. (refer to [Battling](#battle) for more details)

You can initiate an attack at anytime by either assigning individual troops or an entire army. Each troop has their own hp and will regenerate at an intial rate of __4__ hp per day when being attacked. ___Note that other countries will not attack you, only you will be the one attacking___

When you successfully captured 50% of the country's land, you will conquer the country and receives all its buildings only.

## <a name="map"></a>Map design and coordinate system

For __other countries__, you will see the different regions of the other countries represented in coordinates, from A1, A2, to Z10 in text form, with A representing the border of the country and B, C,... being the land deeper into the country. You can choose which land to attack by specifying the coordinate of the land. Same rule applies to battle planning, using the coordinates to specify the attacking order.

For __your own country__, you will not see a coordinate system, but everything combined instead since you will not get attacked.

___Commands:___

- __map__ : _show all countries' name_

- __map [name]__: _show all regions (coordinate) with the terrain and amount of troops for the named country, and the percentage of winning (if in easy difficulty)

## <a name="a3"></a>Technical details
- ### <a name="building"></a>Building
    There are a total of 6 buildings that you can build, each occupying different amount of land and requiring different number of manpower to work on with different time to finish, and having different upgrades:

    _Note the manpower will return once the building is finished. It is not actually gone when you use it. Also, the land usage listed at each stage means what the total amount will be after upgrading, it's not accumulative._

    1. farm (land: 3, manpower: 1, 10 days)
    2. civlian factory (land: 1, manpower: 1, 10 days)
    3. military factory (land: 2, manpower: 2, 15 days)
    4. training camps (land: 1, manpower: 1, 10 days)
    5. infrastructure (land: 1, manpower: 1, 10 days)
    6. airport (land: 1, manpower: 4, 45 days)

    ___Commands:___

    - __build__ : _show all current buildings_

    - __build --help__: _show all buildings and the resources it takes to build
    - __build [name] (amount)__: build the named building type with amount default to 1 if not provided
    - __remove [name] (amount)__: remove the named building type with amount default to 1 if not provided
    - __remove [name] all__: remove all the name building type

    ### __Farm:__

    __To provide food for troops. See [Troops training](#troops) for more info on how much food a unit needs.__

    Stage 1 : land: 3, manpower: 1, 10 days, provide 8 food

    Stage 2 (locked): land: 5, manpower: 1, 10 days, provide 20 food (unlock by [Intermediate Farming research](#s1a))

    Stage 3 (locked): land: 6, manpower: 1, 10 days, provide 28 food (unlock by [Advanced Farming research](#s1b))

    ### __Civilian factory:__

    __To provide the _manpower_ needed to build other buildings. Initial amount varies from levels.__

    Stage 1 : land: 1, manpower: 1, 10 days, provide 1 manpower

    Stage 2 (locked): land: 1, manpower: 1, 15 days, provide 3 manpower (unlock by [Intermediate Division of Labor research](#s2a))

    Stage 3 (locked): land: 2, manpower: 1, 15 days, provide 5 manpower (unlock by [Advanced Division of Labor research](#s2b))

    ### __Military factory:__

    __To provide the _equipment_ needed to increase the strength of the troops.__

    Stage 1 : land: 2, manpower: 2, 15 days, provide 10 equipment

    Stage 2 (locked): land: 3, manpower: 2, 30 days, provide 30 equipment (unlock by [Intermediate Production Line research](#s3a))

    Stage 3 (locked): land: 5, manpower: 4, 60 days, provide 100 equipment (unlock by [Advanced Production Line research](#s3b))

    ### __Training camps:__

    __To train the troops. Each type of troops requires different number of training camps, refer to [Troops training](#troops) for more details.__

    Stage 1 : land: 1, manpower: 1, 10 days

    ### __Infrastructure:__

    __To increase the efficiency of food and equipment delivery to troops, measured by _transportation_. More infrastructure increases the value respectively.__

    Stage 1 : land: 1, manpower: 1, 10 days, 10 transportation

    Stage 2 (locked): land: 1, manpower: 1, 20 days, 25 transportation (unlock by [Intermediate City Planning Research](#s4a))

    Stage 3 (locked): land: 2, manpower: 1, 20 days, 40 transportation (unlock by [Advanced City Planning Research](#s4b))

    ### __Airport:__

    __To allow planes to be built. 1 airport can support exactly _5_ planes, not matter the plane type.__

    Stage 1 : land: 1, manpower: 4, 45 days

- ### <a name="building"></a>Researching
  You can do research anytime during the game __but only one at a time__. Each research will give you different bonuses and unlocks upgrades but also require different time to finish. The following shows all research that you can do in all levels. It always follows the path of rudimentary (what you initally have) to intermediate to advanced.

   ___Commands:___

    - __research__ : _show all researches that can be researched and is researching_

    - __research --help__: _show all researches and the time it takes to build_
    - __research [type]__: _research the area specified by the type. For example, you are rudimentary level in farming and you choose to research farming, then it will research Intermediate Farming research and unlock level 2 upgrade of farms._

  __Farm:__
    - <a name="s1a"></a>Intermediate: 40 days (unlock level 2 farm)
    - <a name="s1b"></a>Advanced: 40 days (unlock level 3 farm)

  __Division of Labor__
    - <a name="s2a"></a>Intermediate: 20 days (unlock level 2 civilian factory)
    - <a name="s2b"></a>Advanced: 30 days (unlock level 3 civilian factory)

  __Production Line__
    - <a name="s3a"></a>Intermediate: 15 days (unlock level 2 military factory)
    - <a name="s3b"></a>Advanced: 30 days (unlock level 3 military factory)

  __City Planning__
    - <a name="s4a"></a>Intermediate: 25 days (unlock level 2 infrastructure)
    - <a name="s4b"></a>Advanced: 25 days (unlock level 3 infrastructure)
  
  __Land Doctrine__
    - Intermediate: 40 days (+20% damage dealt by each unit of land troop)
    - Advanced: 40 days (+30% damage dealt by each unit of land troop)

  __Air Doctrine__
    - Intermediate: 70 days (+40% damage dealt by each plane)
    - Advanced: 100 days (+60% damage dealt by each plane)

  __Urbanization__
    - Intermediate: 50 days (+10% land available for building, rounding off)
    - Advanced: 70 days (+30% land available for building, rounding off)

  __Weapons__
    - Intermediate: 20 days (+5% attacking damage for land troops)
    - Advanced: 70 days (+10% attacking damage for land troops and planes)
  
  __Training__
    - Intermediate: 30 days (-10% training time required to train any unit of land troop)
    - Advanced: 50 days (-20% training time required to train any unit of land troop)

  __Recovery__
    - Intermediate: 30 days (Total 6 recovery per troop per day)
    - Advanced: 50 days (Total 8 recovery per troop per day)
  
- ### <a name="troops"></a>Troops training
   You can train any amount of troop anytime given you have sufficient resources. There is no cap of how many troop you can have, but do make sure you have enough food and equipment to support all the troops (__they consume food even when not in a battle__, and insufficient equipment means lower strength). You may refer [Battling](#battle) for more info on soft attack/ hard attack/ other stats.

   __Any troops may be lost in a battle, so if you see you are getting pushed back (indicated by the decrease of attacking percentage), you should consider pulling back your troops__
   
   The following troops are available for training:

  Land troops:
  1. Infantry
  2. Calvary
  3. Suicide bomber
  4. Sniper
  5. Logistics
  6. Armored cars
  7. Tank 1
  8. Tank 2

  Planes:
  1. Close air support
  2. Fighters
  3. Bombers
  4. Kamikaze

  ___Commands:___

    - __troop__ : _show all trained troops (both land and air) and their status_

    - __train [type] (amount)__: _train the type of troop specified with amount defaulted to 1_

    - __remove [type] (amount)__: _remove the type of troops specified with amount defaulted to 1 (same command used to remove buildings)_

    - __train --help__: _list all troops and their statuses_
   
  Details of each troop (1 unit):
  - __Infantry__: camp: 1, time: 10 days, soft attack: 10, hard attack: 2, defences: 25, speed: 5, equipment: 2, food: 2, hp: 100
  - __Calvary__: camp: 1, time: 10 days, soft attack: 8, hard attack: 1, defences: 20, speed: 10, equipment: 2, food: 2, hp: 150
  - __Suicide bomber__: camp: 2, time: 20 days, soft attack: 30, hard attack: 15, defences: 5, equipment: 10, food: 2 (_suicide bombers are a one-time use, when you send them to battles, they will not return. Also, the damage output will be only __once__._), hp: N/A
  - __Sniper__: camp: 2, time: 20 days, soft attack: 18, hard attack: 1, defences: 1, speed: 2, equipment: 2, food: 1, hp: 30
  - __Logistics__: camp: 1, time: 20 days, equipment: 2, food: 2 (_effect: increase all land troops within the same army's speed by 2 per unit and reduce food consumption by 1, max units per army: 2_), hp: 50
  - __Armored cars__: camp: 2, time: 30 days, soft attack: 18, hard attack: 10, armor: 10, defences: 30, speed: 15, equipment: 10, food: 6, hp: 200
  - __Tank 1__: camp: 3, time: 40 days, soft attack: 12, hard attack: 20, armor: 25, defences: 10, speed: 6, equipment: 15, food: 6, hp: 230
  - __Tank 2__: camp: 5, time: 60 days, soft attack: 12, hard attack: 50, armor: 35, defences: 12, speed: 8, equipment: 25, food: 8, hp: 300
  - __Close Air Support (cas)__: camp: 1, time: 30 days, soft attack: 15, hard attack: 15, air attack: 2, armor: 5, speed: 5, equipment: 10, food: 1 (_to support land troops by soft and hard attack_), hp: 50
  - __Fighters__: camp: 1, time: 30 days, soft attack: 6, hard attack: 6, air attack: 8, armor: 12, speed: 8, equipment: 10, food: 1 (_to support Cas by taking down other enemy planes by air attack_), hp: 130
  - __Bombers__: camp: 2, time: 45 days, soft attack: 6, hard attack: 20, disruption: 10, air attack: 2, armor: 8, speed: 8, equipment: 15, food: 2 (_to distroy enemy's farm/ infrastructure and lower their reinforcement rate and possibly induce attrition_), hp: 80
  - __Kamikaze__: camp: 4, time: 70 days, soft attack: 40, hard attack: 40, disruption: 5, air attack: 2, armor: 8, speed: 8, equipment: 10, food: 1 (_similar to suicide bombers, they will not return from battle and deals only __1__ damage output._), hp: N/A
- ### <a name="army"></a>Army Editing
  You are always encouraged to groups troops trained into army because not only can you use logistics (troop), you can also customize the layout and increase your attacking efficiency by battle plans. You can specify what troops you go first, then what troops stay behind. For example, it may be useful to put infantry/ tanks in the front and sniper at the back. The formation of an army is shown as below.

  D1 C1 B1 A1

  D2 C2 B2 A2

  D3 C3 B3 A3

  D4 C4 B4 A4

  An army can only have a maximum of 16 troops.

  When all troops are up, column A recieves 60% of all attacks while column B gets 30% and C gets the remaining 10% and D receiving none.

  _An attack is received every day in the game (every second in real life)._

  __It is important to note that within each column, how the attack is spread among the 1, 2, 3, 4 troops are completely random. So in extreme cases, A1 may receive most of the 60% attack from the enemy and die very fast.__

  _If more than 1 army is attacking the same region, then the attack from the enemy will first evenly split among the armies, then spread to each troop within the army._

  _You may not need to occupy all the slots within the army, but say if you only put 1 troop in column A, or all except 1 troop in column A died, the remaining 1 troop will take all the 60% attack. So think carefully when editing._ 

   ___Commands:___

    - __army__ : _show all edited armies with their statuses_

    - __army edit [name]__: _edit the army with [name] in __edit mode__. If the army does not exist, a new army will be created._

    - __army remove [name]__: _disband the army with [name]. Note the troops will __not__ be lost._

        _Within the edit mode:_

    - __add [position] [type]__: _assign 1 unit of troop with type specfied to the position specfied (A1, A2...)_

    - __minus [position]__: _unassign the troop at position specified_

    - __reset__: _unassign all positions of the army_

    - __exit__: _save the army template and exit the edit mode_
- ### <a name="plan"></a>Battle planning
    You can devise a battle plan as to specify the order of attacking: which region first, then second and so on. Once you activate the battle plan, the regions specified will be attacked automatically without any need of further actions.

    __If any battle is deemed lost (losing 70% of all troops), or is forced to retreat manually, the troops will fall back to the preivously won region (if any). You can also specify the fallback region if you like during the retreat.__

    __Once fallback starts, the troops will not attack again until you initiate attack again and the battle plan will be scrapped.__

    You __cannot__ assign battle plan to a single troop and can only assign __1__ battle plan for each army.

    To edit a battle plan, you need to first provide the name of the country to be attacked, then the coordinate the region in order you want the army to attack, and the army name you want to assign the battle plan to.

    ___Commands:___

    - __plan__ : _show all battle plans and the associated armies_

    - __plan edit [name]__: _edit the plan that is assigned to the army with [name] in __edit mode__. If the army does have a battle plan, a new plan will be created._

    - __plan remove [name]__: _remove the plan that is assigned to the army with [name]._

        _Within the edit mode:_

    - __target [country]__: _specify the country to be attacking (required before you cna add or minus any region)_

    - __add [region] (order)__: _add the region to be attacked in the order specified (defaulted to at the end of the queue)_

    - __minus [region]__: _remove the region specified_

    - __reset__: _remove all regions in the battle plan_

    - __exit__: _save the plan and exit the edit mode_




