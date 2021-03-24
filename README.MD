# Hearts of Gold

Welcome to a text-based replica of the Hearts of Iron, one of the most intricate game of the century, where you build and devour other countries.

Yung Chin Pang Sam 3035778682<br/>
Wu Ziliang Louis 3035787061

## Content
  - [Goal](#a1)
  - [Progression of game](#a2)
  - [Map design and coordinate system](#map)
  - [Technical details](#a3)
  - [Advice for beginner](#a4)
  - [Other details and tips](#a5)
  - [Saving and Loading](#file)
  - [Random levels](#random)
  - [Commands](#commands)
  - [Implementation](#implementation)
  
## <a name="a1"></a>Goal

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

Researching does not take any resources except time which varies from different types, some may give attack bonus or unlocking building upgrades just to name a few.

To prepare attacking, you must first train some troops. You may group them into different armies to make it more organized.

You may also, but not required, to plan a battle, e.g. immediately attack another region after the current one so no reinforcement will arrive so quickly.

The outcome of the battle depends on the types and amount of troops you and the enemy have, the attack bonuses, the air support and the terrain. (refer to [Battling](#battle) for more details)

You can initiate an attack at anytime by either assigning individual troops or an entire army. Each troop has their own hp and will regenerate at an intial rate of __4__ hp (base) per day when not in full health. <span style="color: tomato">___Note that other countries will not attack you, only you will be the one attacking.___</span>

When you successfully captured 50% of the country's land, you will conquer the country and receives all its buildings only.

___To view your country's status/ resources:___

  ___Commands:___

  - __status__ : _show all statuses, including resources, buildings, troops and regions in battle_

<span style="color: tomato">___The command is automatically executed every 30 seconds.___</span>

## <a name="map"></a>Map design and coordinate system

For __other countries__, you will see the different regions of the other countries represented in coordinates, from A1, A2, to Z10, with A representing the border of the country and B, C,... being the land deeper into the country. __You must first attack the country layer by layer (form A to B to C, etc).__ You can choose which land to attack by specifying the coordinate of the land. Same rule applies to battle planning, using the coordinates to specify the attacking order.

For __your own country__, you will not see a coordinate system, but everything combined instead since you will not get attacked.

__The enemies' map must have all of the regions connected (having at least 1 neighbour, diagonals are not counted)__

Exmaples of enemies' map:

<pre>A1 A2 A3 A4 A5    A7
   B2    B4 B5 B6 B7
   C2 C3 C4 C5    C7
D1 D2       D5</pre>

<pre>A1 A2 A3 
   B2    B4 B5
   C2 C3 C4 C5 C6 C7
D1 D2       D5
E1 E2 E4 E4 E5 E6 E7
F1 F2             F7
G1                G7
H1 H2             H7</pre>

___Commands:___

- __map__ : _show all countries' name_

- __map [name]__: _show all regions (coordinate) with the terrain and amount of troops for the named country, and the percentage of winning (if in easy difficulty)

## <a name="a3"></a>Technical details
- ### <a name="building"></a>Building
    There are a total of 5 buildings that you can build, each occupying different amount of land and requiring different number of manpower to work on with different time to finish, and having different upgrades:

    _Note the manpower will return once the building is finished. It is not actually gone when you use it. Also, the land usage listed at each stage means what the total amount will be after upgrading, it's not accumulative._

    1. farm (land: 3, manpower: 1, 10 days)
    2. civlian factory (land: 1, manpower: 1, 10 days)
    3. military factory (land: 2, manpower: 2, 15 days)
    4. training camps (land: 1, manpower: 1, 10 days)
    5. infrastructure (land: 1, manpower: 1, 10 days)
    6. airport (land: 1, manpower: 4, 45 days)

    ___Commands:___

    - __build__ : _show all current buildings and upgradable buildings_
    - __build --help__: _show all buildings and the resources it takes to build
    - __build [name] (amount)__: build the named building type with amount defaulted to 1 if not provided
    - __upgrade [name] (amount)__: upgrade the named building types with amount defaulted to 1 if not provided
    - __remove [name] (amount)__: remove the named building type with amount defaulted to 1 if not provided
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
  4. Artillery
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
  - __Suicide bomber__: camp: 2, time: 50 days, soft attack: 300, hard attack: 200, defences: 5, equipment: 10, food: 2 (_suicide bombers are a one-time use, when you send them to battles, they will not return. Also, the damage output will be only __once__ , only 1 day._), hp: N/A
  - __Artillery__: camp: 2, time: 20 days, soft attack: 18, hard attack: 1, defences: 1, speed: 2, equipment: 2, food: 1, hp: 30
  - __Logistics__: camp: 1, time: 20 days, equipment: 2, food: 2 (_effect: increase all land troops within the same army's speed by 2 per unit and reduce food consumption by 1, max units per army: 2_), hp: 50
  - __Armored cars__: camp: 2, time: 30 days, soft attack: 18, hard attack: 10, armor: 10, defences: 30, speed: 15, equipment: 10, food: 6, hp: 200
  - __Tank 1__: camp: 3, time: 40 days, soft attack: 12, hard attack: 20, armor: 25, defences: 10, speed: 6, equipment: 15, food: 6, hp: 230
  - __Tank 2__: camp: 5, time: 60 days, soft attack: 12, hard attack: 50, armor: 35, defences: 12, speed: 8, equipment: 25, food: 8, hp: 300
  - __Close Air Support (cas)__: camp: 1, time: 30 days, soft attack: 15, hard attack: 15, air attack: 2, armor: 5, speed: 5, equipment: 10, food: 1 (_to support land troops by soft and hard attack_), hp: 50
  - __Fighters__: camp: 1, time: 30 days, soft attack: 6, hard attack: 6, air attack: 8, armor: 12, equipment: 10, food: 1 (_to support Cas by taking down other enemy planes by air attack_), hp: 130
  - __Bombers__: camp: 2, time: 45 days, soft attack: 6, hard attack: 20, disruption: 10, air attack: 2, armor: 8, equipment: 15, food: 2 (_to distroy enemy's farm/ infrastructure and lower their reinforcement rate and possibly induce attrition_), hp: 80
  - __Kamikaze__: camp: 4, time: 70 days, soft attack: 500, hard attack: 500, disruption: 5, air attack: 150, armor: 8, equipment: 10, food: 1 (_similar to suicide bombers, they will not return from battle and deals only __1__ damage output._), hp: N/A
- ### <a name="army"></a>Army Editing
  You are always encouraged to groups troops trained into army because not only can you use logistics (troop), you can also customize the layout and increase your attacking efficiency by battle plans. You can specify what troops you go first, then what troops stay behind. For example, it may be useful to put infantry/ tanks in the front and Artillery at the back. The formation of an army is shown as below.

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

  ___Note the army name cannot coincide with the any troop's name. e.g. You cannot name the army to infrantry/ Infantry.___

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

    _You can deactivate the battle plan or retreat the army anytime during the battle._

    You __cannot__ assign battle plan to a single troop and can only assign __1__ battle plan for each army.

    To edit a battle plan, you need to first provide the name of the country to be attacked, then the coordinate the region in order you want the army to attack, and the army name you want to assign the battle plan to.

    ___Commands:___

    - __plan__ : _show all battle plans and the associated armies_

    - __plan edit [name]__: _edit the plan that is assigned to the army with [name] in __edit mode__. If the army does have a battle plan, a new plan will be created._

    - __plan remove [name]__: _remove the plan that is assigned to the army with [name]._

        _Within the edit mode:_

    - __target [country]__: _specify the country to be attacking (required before you cna add or minus any region)_

    - __add [region] (order)__: _add the region to be attacked in the order specified (defaulted to at the end of the queue) (__note you must take the land at region A before you can attack region B, refer to [Map design and coordinate system](#map) for more details__)

    - __minus [region]__: _remove the region specified_

    - __reset__: _remove all regions in the battle plan_

    - __exit__: _save the plan and exit the edit mode_
- ### <a name="battle"></a>Battling

    You can initiate a battle at any point in time by assigning armies or individual troops (for those not in an army). A battle is won once all troops on one side in the region die.

    As mentioned before in [Map design and coordinate system](#map),  ___you must start by attacking region A, then B and so on and so forth___. As to what countries you want to invade first, it's entirely up to you.

    You can add in more troops during a battle and withdraw the armies/ troops during the battle. If the army has a battle plan assigned to it, retreating means deactivating the plan. You can also manually deactivate the plan.

    _You cannot, however, edit the army template or the battle plan during battle or retreat particular troops within the army (must retreat the entire army)._

    ___Commands:___

    - __battle status__ : _show all troops in war, available for war, regions currently in battle and their stats_

    - __battle status [region]__ : _show all troops and armies in details, including the remaining troops in the army in different columns and the damage each troops are taking._

    - __battle plan start [name]__: _start the battle plan assigned to the army with [name]_

    - __battle plan stop [name]__: _stop the battle plan assigned to the army with [name] (equivalent to doing battle [region] remove [name])_

    - __battle [country] [region] add (name, (name, ...)) (troop, amount, (troop, amount, ...))__: _attack/ add troops to the [region] in [country] specified with the armies and troops specified (at least 1 unit must be present)_

    - __battle [country] [region] remove (name, (name, ...)) (troop, amount, (troop, amount, ...))__: _remove (retreat) troops from [region] in [country] specified with the armies and troops specified, defaulted to retreat all units if no troops are specified._

  - More details on Attacking/ Defending:
  
    Soft attacks are dealt on all land troops, reduced to a quarter when troop has armor, affected by strength of troop, terrain and disruption.

    Hard attacks __are only dealt on land troops with armor__ (Armored cars and tanks), affected by strength of troop, terrain and disruption. If there are no armor present in the first 3 columns of the enemy army, then no troop will be take damage form the hard attacks.

    Air attacks __are only dealt on planes__.

    Defences are how much attack a troop can sustain before losing hp, affected by strength of troop.

    __An attack is dealt by all troops to enemy and to us once each day. And all troops will recover a base 4 hp per day if hp is not full. (Affected by food supply, disruption and researches.)__

    A troop dies when its hp reaches 0.

    __Formulas for all the items:__

    Supplied food = food required * (total food produced/ total food required (capped at 1, meaning sufficient food for all troops)) __In special cases when you surround 1 enemy region by occupying the 4 adjacent regions, you _cut off_ their supply routes and they receive 0 food no matter of their food production__ (see [Other details and tips](#a5) for more info)

    Supplied equipment is the same as the supplied food.

    Strength of troop = (supplied food/ food required + supplied equipment/ equipment required)/ 2 * 100% (if no food/ equipment is required, then the caluclation will not take the average, but rather 1 fraction or 100%)

    (From the calculation, _all troops_ will suffer from low strength should any of the food or equipment supplied is insufficient.)

    Disruption [![equation-1.png](https://i.postimg.cc/wBfXQFkf/equation-1.png)](https://postimg.cc/jWfnsHmP) (capped at 50%, start at 0%)

    Attack debuff for different terrains (not applicable to planes):

    - Plain: 0%
    - Hill: 2%
    - River: 4%
    - Mountain: 5%
    - Snowland: 8%
    - Desert: 10%

    Attack debuff = base value (terrain) * (1 - speed (troop)/ 50)

    Air supremacy [![equation.png](https://i.postimg.cc/q7khYn7P/equation.png)](https://postimg.cc/PCR5vCYy) (reaches 90% when you have 1.3 times more air attack then the opponent)

    Total air support = Air supremacy * 100 + 10

    Soft attack per troop = base value (troop) * strength * (1.1 bonus damage when in full strength) * (1 - disruption) * (1 - attack debuff)

    Hard attack per troop = base value (troop) * strength * (1.1 bonus damage when in full strength) * (1 - disruption/ 4) * (1 - attack debuff)

    Decrease in hp from land attack per troop (non-armored) = (Total soft attack + air support) / (defending armies + singly defending troops) * (column percentage (in army)) * random value from 0 to 1 (exclusive) (sums up to 1 within the same column) - defences

    Decrease in hp from land attack per troop (armored) = Decrease in hp from attack per troop (non-armored)/4 + (Total hard attack/ number of armored land troops - armor) * 1.1
    
    Decrease in hp from air attack per troop (plane) = Total air attack / number of planes - armor

    Increase in hp from recovery per land troop = base value (research) * (1 - disruption/2) * (food supplied/ food required)^1.5

    Increase in hp from recovery per air troop = base value (research) * (1 - disruption/2) * (food supplied/ food required)^1.5

    (Hence if you surround an enemy region, all troops their will not be able to recover because of 0 food supply)

## <a name="a4"></a>Advice for beginner

- Use the _auto battle_ feature where you designate a target country and the computer will automatically group up all the troops and armies available and try to capture 50% of the country's land (conquering it). Of course it will attack the regions will highest percentage of success but __you risk losing all your troops when all regions have low success rate__. <span style="color: tomato">__You cannot edit armies or battleplans or attack other regions when auto battle is active.__</span> It is automatically deactivated once all of your troops die or the country is captured.

  ___Commands:___

  - __battle [country] auto true__ : _let the computer take control over your troops in attacking the [country]_

  - __battle [country] auto false__ : _take control over your troops_
  
- Choose the easy difficulty as it shows the chance of you capturing a region.
- Always try to make sure you have enough food supply because it affects both your troops' strength and their recovery rate.
- You can focus on only training infantries for the early stages because you can get pretty powerful with having only a lot of infantries.

## <a name="a5"></a>Other details and tips

- Encirclement
  
  You can encircle 1 region of the enemy troops by taking over all 4 adjacent lands, and causing the food supply to drop to 0.

  For examples,

  Taking over C2 in this map will encircle B2.<br>
  Taking over E6 and F7 will encircle E7.<br>
  Taking over C3, D1 and E2 will encircle D2.<br>

  <pre>    A1 A2 A3 
   B2    B4 B5
   C2 C3 C4 C5 C6 C7
   D1 D2       D5
   E1 E2 E4 E4 E5 E6 E7
   F1 F2             F7
   G1                G7
   H1 H2             H7</pre>

   Taking over G2 in this map will encircle G1, G3 and H2 .

  <pre>    A1 A2 A3 
   B2    B4 B5
   C2 C3 C4 C5 C6 C7
   D1 D2       D5
   E1 E2 E4 E4 E5 E6 E7
      F2             F7
   G1 G2 G3          G7
      H2             H7</pre>

## <a name="file"></a>Saving and Loading

You can save the game at anytime using the command

- __save (filename)__: _save the game files to [name].txt format if a filename is provided, save the game files to [level/ name (random levels)]-[yyyy]:[mm]:[dd]:[hh]:[mm]:[ss].txt otherwise_
  
You can also load each level form the files saved using the command

- __load (filename)__: _load the level stored in [name].txt if a filename is provided, load last saved level if otherwise_

## <a name="random"></a>Random levels

You can create a random levels with difficulty 1 to 5 with 1 being the easiest and 5 being the hardest. All maps and resources will be different. The time limit is capped to 30 mins.

__Note there is no guarantee that you can beat the level.__

___Commands:___

- __create [name] (difficulty):__ _create a custom game with [name] (must be unique from other levels) with difficulty defaulted to 1 (easiest) if not provided_

## <a name="commands"></a>Commands
Here's a glossary of commands in random order.
- __status__ : _show all statuses, including resources, buildings, troops and regions in battle_
- __map__ : _show all countries' name_
- __map [name]__: _show all regions (coordinate) with the terrain and amount of troops for the named country, and the percentage of winning (if in easy difficulty)
- __build__ : _show all current buildings and upgradable buildings_
- __build --help__: _show all buildings and the resources it takes to build
- __build [name] (amount)__: build the named building type with amount defaulted to 1 if not provided
- __upgrade [name] (amount)__: upgrade the named building types with amount defaulted to 1 if not provided
- __remove [name] (amount)__: remove the named building type with amount defaulted to 1 if not provided
- __remove [name] all__: remove all the name building type
- __research__ : _show all researches that can be researched and is researching_
- __research --help__: _show all researches and the time it takes to build_
- __research [type]__: _research the area specified by the type._
- __troop__ : _show all trained troops (both land and air) and their status_
- __train [type] (amount)__: _train the type of troop specified with amount defaulted to 1_
- __remove [type] (amount)__: _remove the type of troops specified with amount defaulted to 1 (same command used to remove buildings)_
- __train --help__: _list all troops and their statuses_
- __army__ : _show all edited armies with their statuses_
- __army edit [name]__: _edit the army with [name] in __edit mode__. If the army does not exist, a new army will be created._
- __army remove [name]__: _disband the army with [name]. Note the troops will __not__ be lost._
  
  _Within the army edit mode:_
  - __add [position] [type]__: _assign 1 unit of troop with type specfied to the position specfied (A1, A2...)_
  - __minus [position]__: _unassign the troop at position specified_
  - __reset__: _unassign all positions of the army_
  - __exit__: _save the army template and exit the edit mode_
- __plan__ : _show all battle plans and the associated armies_
- __plan edit [name]__: _edit the plan that is assigned to the army with [name] in __edit mode__. If the army does have a battle plan, a new plan will be created._
- __plan remove [name]__: _remove the plan that is assigned to the army with [name]._
  
  _Within the plan edit mode:_
  - __target [country]__: _specify the country to be attacking (required before you cna add or minus any region)_
  - __add [region] (order)__: _add the region to be attacked in the order specified (defaulted to at the end of the queue) (__note you must take the land at region A before you can attack region B, refer to [Map design and coordinate system](#map) for more details__)
  - __minus [region]__: _remove the region specified_
  - __reset__: _remove all regions in the battle plan_
  - __exit__: _save the plan and exit the edit mode_
- __battle status__ : _show all troops in war, available for war, regions currently in battle and their stats_
- __battle status [region]__ : _show all troops and armies in details, including the remaining troops in the army in different columns and the damage each troops are taking._
- __battle plan start [name]__: _start the battle plan assigned to the army with [name]_
- __battle plan stop [name]__: _stop the battle plan assigned to the army with [name] (equivalent to doing battle [region] remove [name])_
- __battle [country] [region] add (name, (name, ...)) (troop, amount, (troop, amount, ...))__: _attack/ add troops to the [region] in [country] specified with the armies and troops specified (at least 1 unit must be present)_
- __battle [country] [region] remove (name, (name, ...)) (troop, amount, (troop, amount, ...))__: _remove (retreat) troops from [region] in [country] specified with the armies and troops specified, defaulted to retreat all units if no troops are specified._
- __battle [country] auto true__ : _let the computer take control over your troops in attacking the [country]_
- __battle [country] auto false__ : _take control over your troops_
- __speed (1 or 2 or 3)__: _how many days will pass when 1 second passes in real life, defaulted to 1_
- __save (name)__: _save the game files to [name].txt format if a anme is provided, save the game files to [level]-[yyyy]:[mm]:[dd]:[hh]:[mm]:[ss].txt otherwise_
- __load (name)__: _load the level stored in [name].txt if a name is provided, load last saved level if otherwise_
- __create [name] (difficulty):__ _create a custom game with [name] (must be unique from other levels) with difficulty defaulted to 1 (easiest) if not provided_
- __restart__: _restart the level and reset all stats_
- __difficulty (easy or hard)__: _restart the level with difficulty easy/ hard_
- __help__: _print out all commands_

## <a name="implementation"></a>Implementation

Each number corresponds to the coding requirement listed in the file.

1. -> random events through random levels
2. -> STL containers for storing troops, buildings and regions</br>
   -> Classes for individual troop type, building type, and each battle</br>
   -> Common data types such as integers for storing invidual value within each container and object
3. -> By using STL containers such as vectors for purposes stated above</br>
   -> creating new objects form classes/ subclasses</br>
   -> Possibly multithreading from running multiple loops at the same time (may use heap memory instead of stack, not sure)
4. -> Allowing the user to save and load game files from different levels
5. -> Splitting all the classes (troop/ building types) into different header files

### Flow explanation

Flow: menu phase -> action phase starts -> action phase ends -> menu phase ...

A thread is allocated for action phase and the main loop for menu phase will be paused once the actions phase starts.

Implementation for multithreading:<br/>
main thread </br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> calls game thread</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> start timer thread for recording daily (every second) changes </br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> ends game thread when appropriate (illustrated by typing in exit) </br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> ends timer thread when game thread ends
```cpp
#include <iostream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;

class Add {
public:
    int getTime() { return timer; }
    void game()
    {
        // initiate timer thread
        // async to game
        thread thread1(&Add::interval, this);

        while (true) {
            string a;
            getline(cin, a);
            cout << "Inside the class you typed " << a << endl;
            if (a == "exit")
                break;
        }

        //terminate time thread
        this->terminate = true;
        cout << "terminating " << endl;

        // join thread for exiting properly
        thread1.join();
        cout << "game thread id ended: " << this_thread::get_id() << endl;
    }
    void interval()
    {
        while (!terminate) {
            this_thread::sleep_for(chrono::milliseconds(1000));
            timer += 1000;
            cout << "time is " << timer << endl;
        }
        cout << "timer thread id ended: " << this_thread::get_id() << endl;
    }

private:
    int timer = 0;
    bool terminate = false;
};

int main()
{
    string a;
    Add obj;
    bool started = false;
    while (true) {
        getline(cin, a);
        cout << "You typed " << a << endl;
        if (a == "start") {
            thread thread1(&Add::game, obj);
          // main (while loop) pauses
            thread1.join();
        }
        if (a == "exit") {
            cout << "exiting main" << endl;
            break;
        }
    }
    cout << "main thread id ended: " << this_thread::get_id() << endl;
}
```