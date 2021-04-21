# Hearts of Gold

Welcome to a ncurses-free boost-free text-based replica of the Hearts of Iron (not really), one of the most intricate game of the century, where you build and devour other countries.

Yung Chin Pang Sam 3035778682<br/>
Wu Ziliang Louis 3035787061

## Content
  - [Quick Start](#start)
  - [Goal](#a1)
  - [Progression of game](#a2)
  - [Map design and coordinate system](#map)
  - [Technical details](#a3)
  - [Advice for beginner](#a4)
  - [Other details and tips](#a5)
  - [Saving and Loading](#file)
  - [Random levels](#random)
  - [Implementation](#implementation)
  - [Flow](#flow) ( read this before going to the codes )
  - [Testing and development](#development) ( read this before going to the codes )

## <a name="start"></a>Quick Start

Note, this is a linux-version only game and you are to run this game only on linux platforms.

Recommended terminals (restart the terminal if you experience flickering): vscode, putty, ubuntu 18 (system terminal)

First, clone the repo by<br/>
```
git clone https://github.com/samyung0/ENGG1340GP107 hog
```

Then cd in and make (may take around 30s depending on your pc)<br/>
```
cd hog && make
```

Then start the game and enjoy (or suffer depending on how well your play)<br/>
```
./main
```
  
## <a name="a1"></a>Goal

Your objective is to capture all countries in the map __within a time limit__. A country will capitulate once ___70%___ of its land are captured.

___Default game speed is 1 day per second. You may change it in the settings or when in game.___

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

You can initiate an attack at anytime by either assigning individual troops or an entire army. Each troop has their own hp and will regenerate at an intial rate of __1__ hp (base) per day when not in full health. <span style="color: tomato">___Note that other countries will not attack you, only you will be the one attacking.___</span>

___You can attack many blocks at the same time but you can ONLY attack 1 country at each time.___

The outcome of the battle depends on the types and amount of troops you and the enemy have, the attack bonuses, the air support and the terrain, etc. (refer to [Battling](#battle) for more details)

By capturing a block, you will receive the acquirables of it (different buildings and land).

When you have successfully captured 70% of the country's land, you will conquer the country and receives all its acquirables.

## <a name="map"></a>Map design and coordinate system

For __other countries__, you will see the different regions of the other countries represented in coordinates, from A1, A2, to Z10, with A representing the outskirts of the country and B, C,... being the land deeper into the country. __You must attack the country layer by layer (form A to B to C, etc) when pushing inwards.__ You may attack any blocks horizontally. All regions must be at least connected adjacently so there must be a way to attack any blocks eventually.

For __your own country__, you will not see a coordinate system since you will not get attacked.

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

## <a name="a3"></a>Technical details
- ### <a name="building"></a>Building
    There are a total of 5 buildings that you can build, each occupying different amount of land and requiring different number of manpower to work on with different time to finish, and having different upgrades:

    _Note the manpower will return once the building is finished. It is not actually gone when you use it._

    __Also, land is limited and you may need to remove some buildings when insufficient.__

    1. farm (land: 3, manpower: 1, 10 days)
    2. civilian factory (land: 1, manpower: 1, 10 days)
    3. military factory (land: 2, manpower: 2, 15 days)
    4. training camps (land: 1, manpower: 1, 10 days)
    5. airport (land: 1, manpower: 4, 45 days)

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

- ### <a name="research"></a>Researching
  You can do research anytime during the game __but only one at a time__. Each research will give you different bonuses and unlocks upgrades but also require different time to finish. The following shows all research that you can do in all levels. It always follows the path of rudimentary (what you initally have) to intermediate to advanced.

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

  __Weapon__
    - Intermediate: 20 days (+5% attacking damage for tanks)
    - Advanced: 70 days (+10% attacking damage for tanks and planes)
  
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
  5. Logistic
  6. Armored car
  7. Tank 1
  8. Tank 2
  9. Tank oshimai (お終い)

  Planes:
  1. Close air support
  2. Fighter
  3. Bomber
  4. Kamikaze
   
  Details of each troop (1 unit):
  - __Infantry__: camp: 1, time: 10 days, soft attack: 10, hard attack: 2, defenses: 25, speed: 5, equipment: 2, food: 2, hp: 100
  - __Calvary__: camp: 1, time: 10 days, soft attack: 8, hard attack: 1, defenses: 20, speed: 10, equipment: 2, food: 2, hp: 150
  - __Suicide bomber__: camp: 2, time: 50 days, soft attack: 300, hard attack: 200, defenses: 0, speed: 30, equipment: 30, food: 15, disruption: 1 (_suicide bombers are a one-time use and They die immediately after dealing damage once. They ONLY deal damage when they are not taking ANY damage, meaning you must edit them into an army._), hp: N/A
  - __Artillery__: camp: 2, time: 20 days, soft attack: 3, hard attack: 12, defenses: 10, speed: 2, equipment: 2, food: 1, hp: 170
  - __Logistic__: camp: 1, time: 20 days, equipment: 2, defenses: 15, food: 2 (_effect: increase all land troops within the same army's speed by 1 per unit and reduce food consumption by 1, including itself, max units per army: 2_), hp: 50
  - __Armored car__: camp: 2, time: 30 days, soft attack: 18, hard attack: 8, armor: 10, defenses: 30, speed: 15, equipment: 10, food: 6, hp: 160
  - __Tank 1__: camp: 3, time: 40 days, soft attack: 12, hard attack: 20, armor: 25, defenses: 10, speed: 6, equipment: 15, food: 6, hp: 230
  - __Tank 2__: camp: 5, time: 60 days, soft attack: 12, hard attack: 50, armor: 35, defenses: 12, speed: 8, equipment: 25, food: 8, hp: 300
  - __Tank oshimai__: camp: 20, time: 200 days, soft attack: 80, hard attack: 150, armor: 80, defenses: 40, speed: 1, equipment: 150, food: 40, disruption: 2,  hp: 1000
  - __Close Air Support (cas)__: camp: 1, time: 30 days, soft attack: 15, hard attack: 15, air attack: 2, equipment: 10, food: 2, conspicuousness: 2 (_how much attack are spreaded to it_), hp: 50
  - __Fighter__: camp: 1, time: 30 days, soft attack: 4, hard attack: 2, air attack: 8, equipment: 10, food: 5, conspicuousness: 10, hp: 130
  - __Bomber__: camp: 2, time: 45 days, soft attack: 6, hard attack: 20, disruption: 2, air attack: 2, equipment: 15, food: 3, conspicuousness: 6 , hp: 80
  - __Kamikaze__: camp: 4, time: 70 days, soft attack: 500, hard attack: 500, disruption: 5, air attack: 150, equipment: 50, food: 20, conspicuousness: 0 (_similar to suicide bombers, they will not return from battle and only deals damage once given they are not taking damage. Kamikaze does not take any damage when any other planes belonging to you are present._), hp: N/A
- ### <a name="army"></a>Army Editing
  You are always encouraged to groups troops trained into army because not only can you benefit from logistics, you can also customize the layout and sustain the damage output. You can specify which troops are in front and in the back. For example, it may be useful to put tanks in the front and infantries at the back. The formation of an army is shown as below.

  D1 C1 B1 A1

  D2 C2 B2 A2

  D3 C3 B3 A3

  D4 C4 B4 A4

  An army can only have a maximum of 16 troops.

  When all troops are up, column A recieves 60% of all attacks while column B gets 30% and C gets the remaining 10% and D receiving none. Note the multipliers are accumulatable and in the end all 100% damage must be taken (given the presence of land troops) (_Only applicable to soft attacks._)

  __It is important to note that within each column, how the attack is spread among the 1, 2, 3, 4 troops are completely random. There are, however, limits to how extreme the values can be (+-15%). Yet, it may still be possible for a troop to die very fast.__

  _You may not need to occupy all the slots within the army, but say if you only put 1 troop in column A, or all except 1 troop in column A died, the remaining 1 troop will take all the 60% attack. So think carefully when editing._ 

  ___Note the army name must be unique and cannot coincide with any other armies' names.___

- ### <a name="battle"></a>Battling

    You can initiate a battle at any point in time by assigning armies or individual troops (for those not in an army). A battle is won once all troops on one side in the region die or if the block does not have any troops initially.

    As mentioned before in [Map design and coordinate system](#map),  ___you must start by attacking region A, then B and so on and so forth___. As to what countries you want to invade first, it's entirely up to you.

    You can add in more troops during a battle and withdraw the armies/ troops during the battle. _You cannot, however, edit the army template during battle or retreat particular troops within the army (must retreat the entire army)._

  - More details on Attacking/ Defending:
  
    Soft attacks are dealt on all land troops, reduced to a quarter when troop has armor, affected by strength of troop, terrain and disruption.

    Hard attacks __are only dealt on land troops with armor__ (Armored cars and tanks), affected by strength of troop, terrain and disruption. If there are no armor present in the first 3 columns of the enemy army, then no troop will be take damage form the hard attacks.

    Air attacks __are only dealt on planes__.

    Defenses are how much attack a troop can take before losing hp, affected by strength of troop.

    __An attack is dealt by all troops to enemy and to us once each day. And all troops will recover a base 1 hp per day if hp is not full. (Affected by food supply, disruption and researches.)__

    __Formulas for all the items:__

    Supplied food = base food required * (total food produced/ total base food required (capped at 1, meaning sufficient food for all troops)) __In special cases when you surround 1 enemy region by occupying the 4 adjacent blocks, you _cut off_ their supply routes and they receive 0 food no matter of their food production__ (see [Other details and tips](#a5) for more info)

    Supplied equipment calculation is the same as the supplied food.

    Food required = base food required - food reduction (0 reduction if not in army)
    Equipment required = base equipment required - requipment reduction (0 reduction if not in army)

    Subsequential strength = (supplied food/ food required + supplied equipment/ equipment required)/ 2 * 100% (if no food/ equipment is required, then the caluclation will not take the average, but rather 1 fraction or 100%) (equivalent for all troops within the same army)

    Pivotal strength [![render.png](https://i.postimg.cc/g0xGGMPN/render.png)](https://postimg.cc/d7cbBj3y) (100% at full health, minimum ~30%, exponential, hp here = current health/ base health)

    Strength of troop = Pivotal stength * 0.6 + Subsequential strength * 0.4

    (From the calculation, _all troops_ will suffer from low strength should any of the food or equipment supplied is insufficient.)

    Total adjusted disruption [![equation-1.png](https://i.postimg.cc/wBfXQFkf/equation-1.png)](https://postimg.cc/jWfnsHmP) (capped at 50%, start at 0%)

    Attack debuff for different terrains (not applicable to planes):

    - Plain: 0%
    - Hill: 2%
    - River: 4%
    - Mountain: 5%
    - Snowland: 8%
    - Desert: 10%

    Attack debuff = base value (terrain) * (1 - speed (troop)/ 10)

    Air supremacy [![equation.png](https://i.postimg.cc/q7khYn7P/equation.png)](https://postimg.cc/PCR5vCYy) (reaches 90% when you have 1.3 times more air attack then the opponent) (100% if enemy doesn't have plane)

    Normalized air supremacy effect = base value * 0.3 + 0.7 (0.0-1.0 -> 0.7-1.0)

    Air attack = base value (plane) * strength

    Soft attack per troop = base value (troop) * strength * (1.1 bonus damage when in full strength) * (1 - disruption) * (1 - attack debuff) * Normalized air supremacy effect

    Hard attack per troop = base value (troop) * strength * (1.1 bonus damage when in full strength) * (1 - disruption/ 4) * (1 - attack debuff) * Normalized air supremacy effect

    Decrease in hp from land attack per troop (non-armored) = Total soft attack / (defending armies + singly defending troops) * (column percentage (in army)) * random value from 0 to 1 (exclusive) (sums up to 1 within the same column) - defenses

    Decrease in hp from land attack per troop (armored) = Decrease in hp from attack per troop (non-armored)/4 + (Total hard attack/ number of armored land troops - armor) * 1.1
    
    Decrease in hp from air attack per troop (plane) = Total air attack * base conpicuousness/ total conpicuousness

    Increase in hp from recovery per land troop = base value (research) * (1 - disruption/2) * (food supplied/ food required)^1.5

    Increase in hp from recovery per air troop = base value (research) * (1 - disruption/2) * (food supplied/ food required)^1.5

    (Hence if you surround an enemy region, all troops their will not be able to recover because of 0 food supply)

## <a name="a4"></a>Advice for beginner

- pause more often

  The most challenging factor in this game is the time limit, becuase with sufficient time, you can beat any level as long as you can deal some damage because the enemy does not heal when not in battle. You have to carefully plan which block to attack first to reduce the casualties suffered because training new troops are time consuming, and all these thinkings are time-consuming so it is better to pause often.

- aim for 70% captulation rate

  Always go for blocks with less troops. Although many times, they have less acquirables, but the _time-cost_ is lower because less troops will be dead, so no extra training is needed. _Hence_, it is often the case where you will need to aim for capturing 70% of the lands.

- focus on development in the beginning

  You maybe to capture quite a few bits of land in the beginning. But it maybe better to be more organized and do all the buildings in the beginning and launch mass assaults later in the game. One reason is that if you have an army strong enough, you won't suffer any casualties so no extra training is needed. However, if your attack number is similar to the enemies, then you will probably lose a lot of troops even you can capture the block.

- always put in 2 logistics in the back of an army

  You can reduce food required by up to 32 which reduces the need for you to build more farms. Also, putting them in the back lowers the risk of them dying.

## <a name="a5"></a>Other details and tips

- Using Suicide Bomber and Kamikaze

  These 2 troops are considered special troops, and for them to give any damage output, you must have 2 times the number of non-special troops in the battlefield.

- Air Troops Damage Output

  There is a damage dropoff associated with the planes. When you do not have 3 times the number of land troops, the planes damage will start to decrease to 0.

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

- Level number coloring

  Yellow indicates you have beaten the level in easy mode.<br/>
  Green indicates syou have beaten the level in hard mode.

## <a name="random"></a>Random levels

You can create a random levels with difficulty 1 to 5 with 1 being the easiest and 5 being the hardest. All maps and resources will be different. __Time limit is 9999 days. Note, the effect of very large numbers of anything is still not tested.__

## <a name="implementation"></a>Implementation


- Random elements

  - in damage calculation for armies
  - random levels

- STL containers

  - everywhere

- Classes

  - TroopUnit, ArmyUnit, Block, Enemy, Progress, BattleTroopWrapper, BattleUnit, Game

- Structs

  - too many to be listed

- Dynamic memory usages

  - when creating any classes ( raw pointers )
  - when using stl contianers

- Saving and Loading

  - saving mid-game and loading from it
  - saving the completed levels
  - saving the settings

- Multiple files

  - too many to be listed

## <a name="flow"></a>Flow
Term:

- print: printing in terminal
- new: by using dynamic memory
- phase: may contain multiple functions/actions or blocks of codes in different places
<br/>
<br/>

Stage 1: 

  Menu phase</br>
    ↳ Levels ⇆ Setting data origin (level data/ random) ⇆ start Game ➜ stage 2  </br>
    ↳ Settings ⇆ modify settings</br>
    ↳ Stats
<br/>
<br/>
<br/>
Stage 2: Game Phase

Flow: constructor ➜ fetch ➜ start

Constructor Phase
<div style="padding-left: 30px">
↳ creates new <code>resource</code>, <code>building</code>, <code>troop</code>, <code>army</code>, <code>research</code>, <code>battle</code>, <code>enemies</code> ( all structs in namespace data )</br></br>
Resource: store all game stats (food, equipment, multipliers, ...) (self)</br>
Building: store all building stats (built, upgraded, building)</br>
Troop: store all <code>TroopUnit</code> with the number counts (trained, in battle, in army, training)</br>
Army: store all <code>ArmyUnit</code></br>
Research: store all research stats (researched, researching)</br>
Battle: ONLY the general status of whether in battle and is in battle with which country _for printing_</br>
Enemies: store all <code>Enemy</code> and number of defeated enemies</br>
</div>
<br/>
<br/>
Fetch phase
<div style="padding-left: 30px">
↳ populate the structs mentioned before with the data in the level data file or randomly generate them.
</div>
<br/>
<br/>
The four main phases in <code>start phase</code> will be explained below. You may skip them if not interested.
<br/>
<br/>
Printing Phase
<div style="padding-left: 30px">
<ul style="margin:0;list-style-type: bullet">
  <li>main screen</li>
  <li>building screen</li>
  <li>research screen</li>
  <li>training screen</li>
</ul>
Any of the above screen may be printed according to the user input. The screen will be refreshed every frame ( 10 fps ) by looping the print function in a separate thread by calling async and timing it out by using condition variable and wait_for.<br/>
<br/>
Whenever user's input changes the screen, the condition variable for that loop will be notified to quit the loop and start another new loop.
<br/>
<br/>
Reason why condition variable is used instead of simply sleeping the thread is that I need to be able to wake the <span style="font-style:italic">thread from sleeping</span> which is impossible when actually sleeping the thread. I can, however, wait and cancel waiting for condition variable.
<hr>
These functions take in the arrow key position in x and y coordinate, return null.
<hr>
Non-blocking: blocks <code>input phase</code> of Game until the async thread is initiated for setting up the looping<br/>
Data race: lguser locked when setting up the looping, lg3 locked when printing
</div>
<br/>
<br/>
Action Phase
<div style="padding-left: 30px">
<ul style="margin:0;list-style-type: bullet">
  <li>army screen</li>
  <li>battle screen</li>
  <li>changing speed</li>
  <li>pausing</li>
  <li>restart</li>
  <li>quit</li>
</ul>
These actions will only have their functions called once, not looped. Whether the screen will refresh depends on the implementation of that functions.<br>
<code>army screen</code> and <code>battle screen</code> will have their own while loops for <code>input phase</code>, <code>print phase</code> and <code>action phase</code>
<hr>
These functions take in the gamePhase by reference (int, which screen to print or what action to do ) and previous gamePhase, return null. <br>
The actions should finish by setting gamePhase to previous gamePhase because the user should be sent back to his last screen.
<hr>
Non-blocking: blocks <code>input phase</code> of Game<br/>
Data race: handled inside the actions ( some may not, not sure, and thats why the game is prone to crashing )
</div>
<br/>
<br/>
Input Phase
<div style="padding-left: 30px">
Read users input by using getch inside the game while loop ( modifying terminal buffer by using termios )<br/>
up arrow: esc, [, A  <code>^[[A</code><br/>
down arrow: esc, [, B  <code>^[[B</code><br/>
right arrow: esc, [, C  <code>^[[C</code><br/>
left arrow: esc, [, D  <code>^[[D</code>
<br/>
<br/>
Changes gamePhase when user press any shortcut keys or enter.<br>
When input is detected, all current printing loops will be stopped.
<br/>
<br/>
<hr>
Not a function
<hr>
Non-blocking: blocks <code>start phase</code> until user inputs<br/>
Data race: lguser locked when stopping the loop ( and locked when trying to start the loop later ) so data race due to user spamming is avoided 
</div>
<br/>
<br/>
Timer Phase
<div style="padding-left: 30px">
Start the game timer to progress <code>day</code> and <code>battles</code> ( not the battle struct ) until game ends in an async thread.<br/>
Flow: ⟲ increment day<br/> 
      <span style="padding-left: 36px">⟲ start <code>battle cycle phase</code></span></br>
      <span style="padding-left: 36px">⟲ <code>regen (self)</code></span>
<br/>
<br/>
<hr>
Takes in fps and game speed in int, returns null when while loop exit.
<hr>
Non-blocking: non blocking<br/>
Data race: lg locked when increasing day count, refer to <code>battle cycle phase</code> for its own data race handling
</div>
<br/>
<br/>
Start Phase
<div style="padding-left: 30px">
Flow: <code>Timer Phase</code><br/>
<span style="padding-left: 36px">⟲ <code>input</code> ➜ <code>print/action</code></span>
<br/>
Nothing to be exlpained.
<hr>
No parameters, returns 0 ( normal exit ) or -999 ( restart game ) 
<hr>
Non-blocking: blocks <code>game</code> ( but is technically the only function that needs to be run in game )<br/>
Data race: handled inside the phases
</div>
<br/>
<br/>
Battle Cycle Phase
<div style="padding-left: 30px">
Flow: <code>Fight</code> ➜ <code>Clean</code> ➜ <code>Regen</code> ➜ <code>Check End Battle</code><br/>
<br/>
Fight: calculate all stats and damage of the battling troops<br/>
Clean: remove all troops whose health is below 0<br/>
Regen: increment all troops health
Check End Battle: as the function name suggests
<hr>
<code>endGame</code> (reference), gameOver (reference), enemySize, troop (ptr), resource (ptr), building (ptr), battle (ptr), buildBase (reference)
<hr>
Non-blocking: blocks<br/>
Data race: that block's own lg locked
</div>
<br/>
<br/>
endGame
<div style="padding-left: 30px">
Remove all dynamically allocated memories
<hr>
no parameters and no return value
<hr>
Non-blocking: blocks<br/>
Data race: locks everything
</div>
<br/>
<br/>

## <a name="development"></a>Testing and development

- Compiled with:

  - c++11, using g++/gcc

- Tested on:

  - academy server 11
  - ubuntu 18
  - vscode wsl ( ubuntu 18 )

- Memory leaks:

  None by using valgrind

  `valgrind ./main`

- include/lib list

  - core:

    - iostream
    - fstream
    - cmath
    - iomanip
    - cstdlib
    - vector
    - map
    - unordered_map
    - utility
    - functional
    - algorithm
    - string
    - future
    - mutex
    - thread (compiled with -pthread)
    - condition_variable
    - chrono
    - iterator
    - tuple
    - cassert (development purpose)
    - termios (linux only)
    - dirent.h (linus only)
  - external:
    - [uuid by sole](https://github.com/r-lyeh-archived/sole) (used to generate unique uuid)
