#include "Reader.h"
using namespace std;

void Reader::read_map ( Graph* &graph,
                        vector <DrawableObject*> &segments,
                        vector<Wall>* &W,
                        vector<Wall> &player_walls,
                        vector<Color> colors,
                        bool* &show_map)
{
    
    show_map = new bool;
    int N, ratio;
    ifstream file("Input/MAP.txt");
    file >> N >> ratio;
    int x1, y1, x2, y2, c;
    segments.clear();
    vector <Edge> edges(N);
    for (int i = 0; i < N; i++) {
        file >> x1 >> y1 >> x2 >> y2 >> c;
        x1 = x1*ratio + 4;
        y1 = y1*ratio + 4;
        x2 = x2*ratio + 4;
        y2 = y2*ratio + 4;
        edges[i] = {{x1, y1}, {x2, y2}, c == 1 || c == 2, vector <int>(0), c};
        if (c == 1) {
            DrawableEdge *S =  new DrawableEdge(x1, y1, x2, y2, colors[c]);
            S -> visible = show_map;
            segments.push_back(S);
        } else {
            DrawableWall *S =  new DrawableWall(x1, y1, x2, y2, colors[c]);
            segments.push_back(S);
        }
    }
    graph = new Graph(edges);
    W = new vector<Wall>;
    for (int i = 0; i < edges.size(); i++) {
        if (!edges[i].valid)
            W -> push_back({{edges[i].A.first, 0, edges[i].A.second},
                            {edges[i].B.first, 0, edges[i].B.second} });  
        if (edges[i].type != 1)
            player_walls.push_back({{edges[i].A.first, 0, edges[i].A.second},
                                    {edges[i].B.first, 0, edges[i].B.second} });
    }
    file.close();
}

void Reader::read_agents(   Graph* &graph,
                            vector <DrawableObject*> &drawable_agents,
                            Player*    &player,
                            Object*    &coin,
                            Object*    &player_receiver,
                            Object*    &agent_receiver,
                            vector <Wall> &player_walls)
{
    int N;
    ifstream file("Input/Agents.txt");
    file >> N;
    for(int i = 0; i < N; i++) {
        int type;
        file >> type;
        Agent* agent;
        DrawableObject* drawable_agent;
        double x, y, z, speed;
        int size;
        
        if (type == 0) {
            file >> x >> y >> z >> speed >> size;
            player = new Player(x, y, z, speed);
            drawable_agent = new DrawableAgent(player->character, Color::White, size);
            drawable_agents.push_back(drawable_agent);
            int N, size;
            file >> N >> size;
            (player -> entry_portals).resize(4);
            (player -> exit_portals).resize(4);
            (player -> permutation).resize(4);
            for (int i = 0; i < N; i++){
                double x1, z1, x2, z2;
                file >> x1 >> z1 >> x2 >> z2;
                (player -> entry_portals)[i]    = {x1, 0, z1};
                (player -> exit_portals)[i]     = {x2, 0, z2};
                DrawablePortal *drawable_entry_portal, *drawable_exit_portal;
                drawable_entry_portal   = new DrawablePortal(x1, z1, size, Color::Red);
                drawable_exit_portal    = new DrawablePortal(x2, z2, size, Color::Blue);
                drawable_agents.push_back(drawable_entry_portal);
                drawable_agents.push_back(drawable_exit_portal);
            }
            for (int i = 0; i < N; i++)
                file >> (player -> permutation)[i];
            player -> graph = graph;
        }else if(type == 1) {
            file >> size;
            coin = new Object();
            drawable_agent = new DrawableAgent(coin->character, Color::Yellow, size);
            drawable_agent -> visible = &(coin -> exists);
            drawable_agents.push_back(drawable_agent);
        }else if(type == 2) {
            file >> size;
            player_receiver = new Object();
            drawable_agent  = new DrawableAgent(player_receiver -> character, Color::Blue, size);
            drawable_agent -> visible = &(player_receiver -> exists);
            drawable_agents.push_back(drawable_agent);  
        } else if(type == 3) {
            file >> size;
            agent_receiver = new Object();
            drawable_agent = new DrawableAgent(agent_receiver -> character, Color::Green, size);
            drawable_agent -> visible = &(agent_receiver -> exists);
            drawable_agents.push_back(drawable_agent);
        }
    }
    player -> walls = player_walls;
    file.close();
}

void Reader::read_competitor(   Graph* &graph,
                                Character* &competitor,
                                vector<Wall>* &walls,
                                Player* &player,
                                Object* &coin,
                                double* &time,
                                vector<DrawableObject*> &drawable_agents)
{
    
    ifstream file("Input/competitor.txt");
    time = new double();
    double epsilon;
    file >> epsilon;

    /**************** COMPETIDOR ************/
    competitor = new Character();
    double x, z, max_speed;
    file >> x >> z >> max_speed;
    (competitor -> character).position = {x, 0, z};
    (competitor -> character).max_speed = max_speed;
    competitor -> graph = graph;
    
    // Evitar paredes
    double lookahead, avoid_distance, max_acc_obstacle;
    file >> lookahead >> avoid_distance >> max_acc_obstacle;
    ObstacleAvoidance* obstacle_avoidance = new ObstacleAvoidance();
    obstacle_avoidance -> lookahead = lookahead;
    obstacle_avoidance -> avoid_distance = avoid_distance;
    obstacle_avoidance -> max_acceleration = max_acc_obstacle;
    (obstacle_avoidance -> collision_detector).walls = walls;

    /*************************SEGUIR MONEDA ********************/
    //Seguir moneda
    double path_offset, max_acc_follow_path;
    file >> path_offset >> max_acc_follow_path;
    FollowPath* follow_path = new FollowPath();
    follow_path -> path_offset = path_offset;
    follow_path -> max_acceleration = max_acc_follow_path;
    follow_path -> character = &(competitor -> character);

    //Arrive
    double max_acc_arrive, max_speed_arrive, target_radius, slow_radius, time_to_target;
    file >> max_acc_arrive >> max_speed_arrive;
    file >> target_radius >> slow_radius >> time_to_target;
    Arrive *arrive = new Arrive();
    arrive -> target = &(coin -> character.position);
    arrive -> max_acceleration = max_acc_arrive;
    arrive -> max_speed = max_speed_arrive;
    arrive -> target_radius = target_radius;
    arrive -> slow_radius = slow_radius;
    arrive -> time_to_target = time_to_target;

    // Crear los steering que se utilizaran en la accion finding_coin
    PrioritySteering *priority_steering_coin    = new PrioritySteering();
    (priority_steering_coin -> behaviours).resize(3);
    (priority_steering_coin -> behaviours)[0] = obstacle_avoidance;
    (priority_steering_coin -> behaviours)[1] = follow_path;
    (priority_steering_coin -> behaviours)[2] = arrive;
    priority_steering_coin -> character = &(competitor -> character);
    priority_steering_coin -> epsilon   = epsilon;



    /*********************** FOLLOW SMELL *********************/

    // Seek
    double max_acc_seek;
    file >> max_acc_seek;
    Seek *seek = new Seek();
    seek -> target = new Vector3 <double> ((competitor -> character).position);
    seek -> max_acceleration = max_acc_seek;
    seek -> character = &(competitor -> character);

    PrioritySteering *priority_steering_smell = new PrioritySteering();
    (priority_steering_smell -> behaviours).resize(2);
    (priority_steering_smell-> behaviours)[0] = obstacle_avoidance;
    (priority_steering_smell-> behaviours)[1] = seek;
    priority_steering_smell -> character = &(competitor -> character);
    priority_steering_smell -> epsilon   = epsilon;


    /************************** ACCIONES DE ESTADOS *******************/
    int *node = new int();
    FollowSmell* following_smell = new FollowSmell();
    following_smell -> time = time;
    following_smell -> graph = graph;
    following_smell -> steering_behavior = priority_steering_smell;
    following_smell -> seek = seek;
    following_smell -> node = node;


    SteeringBehaviorAction *finding_coin = new SteeringBehaviorAction();
    finding_coin -> steering_behavior   = priority_steering_coin;
    finding_coin -> time                = time;

    /************************ ACCIONES DE TRANSICIONES ***************/
    FindNode *find_node = new FindNode();
    find_node -> seek = seek;
    find_node -> node = node;
    find_node -> graph = graph;

    FindBestPath* calculate_path = new FindBestPath();
    calculate_path -> follow_path   = follow_path;
    calculate_path -> graph         = graph;
    calculate_path -> target        = &(coin -> character).position;

    Action *none = new Action();

    /*********************** CONDICIONES ****************************/
    BoolCondition *check_coin = new BoolCondition();
    NotCondition  *not_coin   = new NotCondition();
    check_coin -> condition = &(coin -> exists);
    not_coin ->   condition = check_coin;


    /******************************* MAQUINA DE ESTADOS ************/
    (competitor -> state_machine).states.resize(2);
    State &find_coin = (competitor -> state_machine).states[0];
    State &follow_smell = (competitor -> state_machine).states[1];

    (competitor -> state_machine).initial_state = &follow_smell;
    (competitor -> state_machine).current_state = &follow_smell;

    /************************** ESTADOS *******************************/
    find_coin.action    = finding_coin;
    follow_smell.action  = following_smell;
    find_coin.transitions.push_back({&follow_smell, not_coin, find_node});
    follow_smell.transitions.push_back({&find_coin, check_coin, calculate_path});

    /******************* OBJETO DIBUJABLE **************************/
    int size;
    file >> size;
    DrawableObject* drawable_agent;
    drawable_agent = new DrawableAgent(competitor->character, Color::Magenta, size);
    drawable_agents.push_back(drawable_agent);
}

void Reader::read_vigilant( Graph* &graph,
                            Character* &vigilant,
                            // Character* &competitor,
                            vector<Wall>* &walls,
                            Player* &player,
                            Object* &coin,
                            double* &time,
                            vector<DrawableObject*> &drawable_agents)
{
    ifstream file("Input/vigilant.txt");
    double epsilon;
    file >> epsilon;

    /******************************* VIGILANT **********************/
    vigilant = new Character();
    double x, z, max_speed;
    file >> x >> z >> max_speed;
    (vigilant -> character).position = {x, 0, z};
    (vigilant -> character).max_speed = max_speed;
    vigilant -> graph = graph;

    /************************ EVITAR PAREDES **********************/
    // Evitar paredes
    double lookahead, avoid_distance, max_acc_obstacle;
    file >> lookahead >> avoid_distance >> max_acc_obstacle;
    ObstacleAvoidance* obstacle_avoidance = new ObstacleAvoidance();
    obstacle_avoidance -> lookahead = lookahead;
    obstacle_avoidance -> avoid_distance = avoid_distance;
    obstacle_avoidance -> max_acceleration = max_acc_obstacle;
    (obstacle_avoidance -> collision_detector).walls = walls;

    /******************************  DESCANSAR *******************/

    // Hacer Arrive
    double max_acc_arrive, max_speed_arrive, target_radius, slow_radius, time_to_target;
    file >> max_acc_arrive >> max_speed_arrive;
    file >> target_radius >> slow_radius >> time_to_target;
    Arrive *arrive = new Arrive();
    arrive -> target = &(vigilant -> character.position);
    arrive -> max_acceleration = max_acc_arrive;
    arrive -> max_speed = max_speed_arrive;
    arrive -> target_radius = target_radius;
    arrive -> slow_radius = slow_radius;
    arrive -> time_to_target = time_to_target;

    // Hacer priority steering
    PrioritySteering *priority_steering_rest    = new PrioritySteering();
    (priority_steering_rest -> behaviours).resize(2);
    (priority_steering_rest -> behaviours)[0] = obstacle_avoidance;
    (priority_steering_rest -> behaviours)[1] = arrive;
    priority_steering_rest -> character = &(vigilant -> character);
    priority_steering_rest -> epsilon   = epsilon;
    /*********************** VIGILAR EN LA MISMA ZONA *******************/
    // Seek
    double max_acc_seek;
    file >> max_acc_seek;
    Seek *seek = new Seek();
    seek -> target = new Vector3 <double> ((vigilant -> character).position);
    seek -> max_acceleration = max_acc_seek;
    seek -> character = &(vigilant -> character);

    // Hacer priority steering
    PrioritySteering *priority_steering_guard = new PrioritySteering();
    (priority_steering_guard -> behaviours).resize(2);
    (priority_steering_guard-> behaviours)[0] = obstacle_avoidance;
    (priority_steering_guard-> behaviours)[1] = seek;
    priority_steering_guard -> character = &(vigilant -> character);
    priority_steering_guard -> epsilon   = epsilon;

    /************************* MOVERSE A OTRA POSICION ********************/
    // follow path

    double path_offset, max_acc_follow_path;
    file >> path_offset >> max_acc_follow_path;
    FollowPath* follow_path = new FollowPath();
    follow_path -> path_offset = path_offset;
    follow_path -> max_acceleration = max_acc_follow_path;
    follow_path -> character = &(vigilant -> character);

    PrioritySteering *priority_steering_move = new PrioritySteering();
    (priority_steering_move -> behaviours).resize(2);
    (priority_steering_move-> behaviours)[0] = obstacle_avoidance;
    (priority_steering_move-> behaviours)[1] = follow_path;
    priority_steering_move -> character = &(vigilant -> character);
    priority_steering_move -> epsilon   = epsilon;

    /***************************** PERSEGUIR AL JUGADOR **********************/
    // seek
    double max_acc_seek_player;
    file >> max_acc_seek_player;
    Seek *seek_player = new Seek();
    seek_player -> target = &(player -> character).position;
    seek_player -> max_acceleration = max_acc_seek_player;

    PrioritySteering *priority_steering_seek_player = new PrioritySteering();
    (priority_steering_seek_player -> behaviours).resize(2);
    (priority_steering_seek_player-> behaviours)[0] = obstacle_avoidance;
    (priority_steering_seek_player-> behaviours)[1] = seek_player;
    priority_steering_seek_player -> character = &(vigilant -> character);
    priority_steering_seek_player -> epsilon   = epsilon;


    /**************************** ACCIONES DE ESTADOS **********************/

    // Nada
    Action* none = new Action();

    // Descansar
    SteeringBehaviorAction *resting = new SteeringBehaviorAction();
    resting -> steering_behavior   = priority_steering_rest;
    resting -> time                = time;

    // Vigilar
    int *node = new int();
    RandomMovement* random_moving = new RandomMovement();
    random_moving -> time = time;
    random_moving -> graph = graph;
    random_moving -> steering_behavior = priority_steering_guard;
    random_moving -> seek = seek;
    random_moving -> node = node;

    // Moverse
    SteeringBehaviorAction *moving  = new SteeringBehaviorAction();
    moving -> steering_behavior     = priority_steering_move;
    moving -> time                  = time;

    // Perseguir jugador
    SteeringBehaviorAction *seeking = new SteeringBehaviorAction();
    seeking -> steering_behavior    = priority_steering_seek_player;
    seeking -> time                 = time;

    /************************ ACCIONES DE TRANSICIONES *****************/
    FindNode *find_node = new FindNode();
    find_node -> seek = seek;
    find_node -> node = node;
    find_node -> graph = graph;


    ChooseRandomPosition *choose_position = new ChooseRandomPosition();
    choose_position -> graph = graph;

    FindBestPath* calculate_path = new FindBestPath();
    calculate_path -> follow_path   = follow_path;
    calculate_path -> graph         = graph;
    calculate_path -> target        = &(choose_position -> target);

    MultipleActionsAction *create_path = new MultipleActionsAction();
    (create_path -> actions).resize(2);
    (create_path -> actions)[0] = choose_position;
    (create_path -> actions)[1] = calculate_path;


    /**************************** CONDICIIONES **************************/
    RandomCondition* random_rest_guard = new RandomCondition();
    int p_rg, q_rg;
    file >> p_rg >> q_rg;
    random_rest_guard -> p = p_rg;
    random_rest_guard -> q = q_rg;

    RandomCondition* random_guard_rest = new RandomCondition();
    int p_gr, q_gr;
    file >> p_gr >> q_gr;
    random_guard_rest -> p = p_gr;
    random_guard_rest -> q = q_gr;

    BoolCondition* always_true = new BoolCondition();
    always_true -> condition = new bool(true);

    RandomCondition* change_position = new RandomCondition();
    int p_cp, q_cp;
    file >> p_cp >> q_cp;
    change_position -> p = p_cp;
    change_position -> q = q_cp;

    BoolCondition* end_path = new BoolCondition();
    end_path -> condition = &(follow_path -> end_path);

    double lookahead_follow_player;
    file >> lookahead_follow_player;
    SeeTargetCondition* near_player  = new SeeTargetCondition();
    near_player -> character     = &(vigilant -> character);
    near_player -> target        = &(player -> character).position;
    near_player -> lookahead     = new double (lookahead_follow_player);

    IntegersMatchCondition* same_section = new IntegersMatchCondition();
    same_section -> watch1 = &(player -> section);
    same_section -> watch2 = &(vigilant -> section);

    AndCondition* see_player = new AndCondition();
    see_player -> condition1 = near_player;
    see_player -> condition2 = same_section;

    NotCondition* no_see_player = new NotCondition();
    no_see_player -> condition = see_player;


    /********************************* MAQUINA DE ESTADOS **********************/
    (vigilant -> state_machine).states.resize(5);
    State &initial_state    = (vigilant -> state_machine).states[0];
    State &rest             = (vigilant -> state_machine).states[1];
    State &guard            = (vigilant -> state_machine).states[2];
    State &move             = (vigilant -> state_machine).states[3];
    State &follow_player    = (vigilant -> state_machine).states[4];

    (vigilant -> state_machine).initial_state = &initial_state;
    (vigilant -> state_machine).current_state = &initial_state;

    /************************** ESTADOS *******************************/

    initial_state.action    = none;
    rest.action             = resting;
    guard.action            = random_moving;
    move.action             = moving;
    follow_player.action    = seeking;

    initial_state.transitions.push_back({&guard, always_true, find_node});
    rest.transitions.push_back({&follow_player, see_player, none});
    rest.transitions.push_back({&guard, random_rest_guard, none});
    guard.transitions.push_back({&follow_player, see_player, none});
    guard.transitions.push_back({&rest, random_guard_rest, none});
    guard.transitions.push_back({&move, change_position, create_path});
    move.transitions.push_back({&follow_player, see_player, none});
    move.transitions.push_back({&guard, end_path, find_node});
    follow_player.transitions.push_back({&guard, no_see_player, find_node});

    int size;
    file >> size;
    DrawableObject* drawable_agent;
    drawable_agent = new DrawableAgent(vigilant->character, Color::Cyan, size);
    drawable_agents.push_back(drawable_agent);
}

void Reader::read_friends(    Graph* &graph,
                                // Character* &vigilant,
                                Character* &competitor,
                                vector<Wall>* &walls,
                                // Player* &player,
                                // Object* &coin,
                                double* &time,
                                vector<Friend*> &friends,
                                vector<DrawableObject*> &drawable_agents)
{
    ifstream file("Input/friends.txt");
    int N;
    file >> N;
    double epsilon;
    file >> epsilon;

    for (int _ = 0; _ < N; _++) {

        /**************************** FRIEND **************************/
        Friend* friend_player = new Friend();

        double x, z, max_speed;
        int section;
        file >> x >> z >> max_speed >> section;
        (friend_player -> character).position = {x, 0, z};
        (friend_player -> character).max_speed = max_speed;
        friend_player -> section = section;

        /************************ EVITAR PAREDES **********************/
        // Evitar paredes
        double lookahead, avoid_distance, max_acc_obstacle;
        file >> lookahead >> avoid_distance >> max_acc_obstacle;
        ObstacleAvoidance* obstacle_avoidance = new ObstacleAvoidance();
        obstacle_avoidance -> lookahead = lookahead;
        obstacle_avoidance -> avoid_distance = avoid_distance;
        obstacle_avoidance -> max_acceleration = max_acc_obstacle;
        (obstacle_avoidance -> collision_detector).walls = walls;


        /************************* PERSEGUIR COMPETIDOR ****************/
        // seek
        double max_acc_seek;
        file >> max_acc_seek;
        Seek *seek = new Seek();
        seek -> target = &(competitor -> character).position;
        seek -> max_acceleration = max_acc_seek;

        PrioritySteering *priority_steering_seek = new PrioritySteering();
        (priority_steering_seek -> behaviours).resize(2);
        (priority_steering_seek-> behaviours)[0] = obstacle_avoidance;
        (priority_steering_seek-> behaviours)[1] = seek;
        priority_steering_seek -> character = &(friend_player -> character);
        priority_steering_seek -> epsilon   = epsilon;

        /************************* MOVERSE A LA SOMBRA ********************/
        // follow path
        double path_offset, max_acc_follow_path;
        file >> path_offset >> max_acc_follow_path;
        FollowPath* follow_path = new FollowPath();
        follow_path -> path_offset = path_offset;
        follow_path -> max_acceleration = max_acc_follow_path;
        follow_path -> character = &(friend_player -> character);

        //arrive
        double max_acc_arrive, max_speed_arrive, target_radius, slow_radius, time_to_target;
        file >> max_acc_arrive >> max_speed_arrive;
        file >> target_radius >> slow_radius >> time_to_target;
        Arrive *arrive = new Arrive();
        arrive -> target = &(friend_player -> shadow_point);
        arrive -> max_acceleration = max_acc_arrive;
        arrive -> max_speed = max_speed_arrive;
        arrive -> target_radius = target_radius;
        arrive -> slow_radius = slow_radius;
        arrive -> time_to_target = time_to_target;

        PrioritySteering *priority_steering_move = new PrioritySteering();
        (priority_steering_move -> behaviours).resize(3);
        (priority_steering_move-> behaviours)[0] = obstacle_avoidance;
        (priority_steering_move-> behaviours)[1] = follow_path;
        (priority_steering_move-> behaviours)[2] = arrive;
        priority_steering_move -> character = &(friend_player -> character);
        priority_steering_move -> epsilon   = epsilon;

        /**************************** ACCIONES DE ESTADOS **********************/
        // Nada
        Action* none = new Action();

        //Perseguir al competidor
        SteeringBehaviorAction *seeking = new SteeringBehaviorAction();
        seeking -> steering_behavior    = priority_steering_seek;
        seeking -> time                 = time;

        SteeringBehaviorAction *moving  = new SteeringBehaviorAction();
        moving -> steering_behavior     = priority_steering_move;
        moving -> time                  = time;

        /************************ ACCIONES DE TRANSICIONES *****************/
        FindBestPath* calculate_path = new FindBestPath();
        calculate_path -> follow_path   = follow_path;
        calculate_path -> graph         = graph;
        calculate_path -> target        = &(friend_player -> shadow_point);

        /**************************** CONDICIONES **************************/
        IntegersMatchCondition *same_section = new IntegersMatchCondition();
        same_section -> watch1 = &(competitor -> section);
        same_section -> watch2 = &(friend_player -> section);
        NotCondition *distinct_section = new NotCondition();
        distinct_section -> condition = same_section;


        BoolCondition* always_true = new BoolCondition();
        always_true -> condition = new bool(true);

        /********************************* MAQUINA DE ESTADOS **********************/
        (friend_player -> state_machine).states.resize(3);
        State &initial_state    = (friend_player -> state_machine).states[0];
        State &seek_competitor  = (friend_player -> state_machine).states[1];
        State &move_shadow      = (friend_player -> state_machine).states[2];


        (friend_player -> state_machine).initial_state = &initial_state;
        (friend_player -> state_machine).current_state = &initial_state;

        /****************************** ESTADOS *****************************/
        initial_state.action    = none;
        seek_competitor.action  = seeking;
        move_shadow.action      = moving;

        initial_state.transitions.push_back({&move_shadow, always_true, calculate_path});
        seek_competitor.transitions.push_back({&move_shadow, distinct_section, calculate_path});
        move_shadow.transitions.push_back({&seek_competitor, same_section, none});

        friends.push_back(friend_player);

        /********************************** OBJETO DIBUJABLE ********************/
        int size;
        file >> size;
        DrawableObject* drawable_agent;
        drawable_agent = new DrawableAgent(friend_player->character, Color(211, 211, 211), size);
        drawable_agents.push_back(drawable_agent);
    }
    file.close();
}