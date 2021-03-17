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

You can initiate an attack at anytime by either assigning individual troops or an entire army. ___But note that other countries will not attack you, only you will be the one attacking___

When you successfully captured 50% of the country's land, you will conquer the country and receives all its buildings only.

## <a name="map"></a>Map design and coordinate system
For __other countries__, you will see the different regions of the other countries represented in coordinates, from A1, A2, to Z10 in text form. You can see some details of the land (defences/ buildings/ terrain, refer to [Researching](#research) for more info). You can choose which land to attack by specifying the coordinate of the land. Same rule applies to battle planning, using the coordinates to specify the attacking order.

For __your own country__, you will not see a coordinate system, but everything combined instead since you will not get attacked.

## <a name="a3"></a>Technical details
- ### <a name="building"></a>Building
    There are a total of 8 buildings that you can build, each occupying different amount of land and requiring different number of manpower to work on with different time to finish, and having different upgrades:

    _Note the manpower will return once the building is finished. It is not actually gone when you use it. Also, the land usage listed at each stage means what the total amount will be after upgrading, it's not accumulative._

    1. farm (land: 3, manpower: 1, 10 days)
    2. civlian factory (land: 1, manpower: 1, 10 days)
    3. military factory (land: 2, manpower: 2, 15 days)
    4. training camps (land: 1, manpower: 1, 10 days)
    5. research center (land: 2, manpower: 4, 30 days)
    6. infrastructure (land: 1, manpower: 1, 10 days)
    7. airport (land: 1, manpower: 4, 45 days)

    ### __Farm:__

    __To provide food for troops. See [Troops training](#troops) for more info on how much food a unit needs.__

    Stage 1 : land: 3, manpower: 1, 10 days, provide 8 food

    Stage 2 (locked): land: 5, manpower: 1, 10 days, provide 20 food (unlock by Intermediate Farming research)

    Stage 3 (locked): land: 6, manpower: 1, 10 days, provide 28 food (unlock by Advanced Farming research)

    ### __Civilian factory:__

    __To provide the _manpower_ needed to build other buildings. Initial amount varies from levels.__

    Stage 1 : land: 1, manpower: 1, 10 days, provide 1 manpower

    Stage 2 (locked): land: 1, manpower: 1, 15 days, provide 3 manpower (unlock by Intermediate Division of Labor research)

    Stage 3 (locked): land: 2, manpower: 1, 15 days, provide 5 manpower (unlock by Advanced Division of Labor research)

    ### __Military factory:__

    __To provide the _equipment_ needed to increase the strength of the troops.__

    Stage 1 : land: 2, manpower: 2, 15 days, provide 10 equipment

    Stage 2 (locked): land: 3, manpower: 2, 30 days, provide 30 equipment (unlock by Intermediate Production Line research)

    Stage 3 (locked): land: 5, manpower: 4, 60 days, provide 100 equipment (unlock by Advanced Production Line research)

    ### __Training camps:__

    __To train the troops. Each type of troops requires different number of training camps, refer to [Troops training](#troops) for more details.__

    Stage 1 : land: 1, manpower: 1, 10 days

    ### __Research center:__

    __To provide researching power for researches to take place. Each research requires different amount of researching power and time, refer to [Researching](#research) for more details__

    Stage 1 : land: 2, manpower: 4, 30 days, 10 researching power

    Stage 2 : land: 2, manpower: 2, 45 days, provide 30 researching power

    Stage 3 : land: 3, manpower: 2, 45 days, provide 50 researching power

    ### __Infrastructure:__

    __To increase the efficiency of food and equipment delivery to troops, measured by _transportation_. More infrastructure increases the value respectively.__

    Stage 1 : land: 1, manpower: 1, 10 days, 10 transportation

    Stage 2 (locked): land: 1, manpower: 1, 20 days, 25 transportation (unlock by Intermediate City Planning Research)

    Stage 3 (locked): land: 2, manpower: 1, 20 days, 40 transportation (unlock by Advanced City Planning Research)

    ### __Airport:__

    __To allow planes to be built. 1 airport can support exactly _5_ planes, not matter the plane type.__

    Stage 1 : land: 1, manpower: 4, 45 days