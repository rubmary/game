#include "Reader.h"
using namespace std;

void Reader::read_map ( Graph* &graph,
                        vector <DrawableObject*> &segments,
                        vector<Wall>* &W,
                        vector<Color> colors,
                        bool* &show_map)
{
    
    show_map = new bool;
    int N, ratio;
    ifstream file("MAP.txt");
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
            W -> push_back({ {edges[i].A.first, 0, edges[i].A.second},
                          {edges[i].B.first, 0, edges[i].B.second} });
    }
    file.close();
}

void Reader::read_agents(vector <DrawableObject*> &drawable_agents,
                         Player*    &player,
                         Object*    &coin,
                         Object*    &player_receiver,
                         Object*    &agent_receiver)
{
    int N;
    ifstream file("Agents.txt");
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
    
    ifstream file("competitor.txt");
    time = new double();
    double epsilon;
    file >> epsilon;

    /**************** COMPETIDOR ************/
    competitor = new Character();
    double x, z, max_speed;
    file >> x >> z >> max_speed;
    (competitor -> character).position = {x, 0, z};
    (competitor -> character).max_speed = max_speed;

    cout << "Hice competidor" << endl;
    
    // Evitar paredes
    double lookahead, avoid_distance, max_acc_obstacle;
    file >> lookahead >> avoid_distance >> max_acc_obstacle;
    ObstacleAvoidance* obstacle_avoidance = new ObstacleAvoidance();
    obstacle_avoidance -> lookahead = lookahead;
    obstacle_avoidance -> avoid_distance = avoid_distance;
    obstacle_avoidance -> max_acceleration = max_acc_obstacle;
    (obstacle_avoidance -> collision_detector).walls = walls;


    cout << "Hice evitador de obstaculos" << endl;

    /*************************SEGUIR MONEDA ********************/
    //Seguir moneda
    double path_offset, max_acc_follow_path;
    file >> path_offset >> max_acc_follow_path;
    FollowPath* follow_path = new FollowPath();
    follow_path -> path_offset = path_offset;
    follow_path -> max_acceleration = max_acc_follow_path;
    follow_path -> character = &(competitor -> character);
    cout << "Hice seguir moneda" << endl;

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

    cout << "Hice arrive" << endl;
    // Crear los steering que se utilizaran en la accion finding_coin
    PrioritySteering *priority_steering_coin    = new PrioritySteering();
    (priority_steering_coin -> behaviours).resize(3);
    (priority_steering_coin -> behaviours)[0] = obstacle_avoidance;
    (priority_steering_coin -> behaviours)[1] = follow_path;
    (priority_steering_coin -> behaviours)[2] = arrive;
    priority_steering_coin -> character = &(competitor -> character);
    priority_steering_coin -> epsilon   = epsilon;

    cout << "Primer priority steering" << endl;


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

    cout << "Seek" << endl;

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

    cout << "Acciones de estados" << endl;
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
    cout << "Acciones de transiciones" << endl;

    /*********************** CONDICIONES ****************************/
    BoolCondition *check_coin = new BoolCondition();
    NotCondition  *not_coin   = new NotCondition();
    check_coin -> condition = &(coin -> exists);
    not_coin ->   condition = check_coin;

    cout << "Condiciones" << endl;

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
    cout << "maquina de estados" << endl;

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
    ifstream file("vigilant.txt");
    double epsilon;
    file >> epsilon;

    /******************************* VIGILANT **********************/
    vigilant = new Character();
    double x, z, max_speed;
    file >> x >> z >> max_speed;
    (vigilant -> character).position = {x, 0, z};
    (vigilant -> character).max_speed = max_speed;

    cout << "Hice vigilante" << endl;

    /************************ EVITAR PAREDES **********************/
    // Evitar paredes
    double lookahead, avoid_distance, max_acc_obstacle;
    file >> lookahead >> avoid_distance >> max_acc_obstacle;
    ObstacleAvoidance* obstacle_avoidance = new ObstacleAvoidance();
    obstacle_avoidance -> lookahead = lookahead;
    obstacle_avoidance -> avoid_distance = avoid_distance;
    obstacle_avoidance -> max_acceleration = max_acc_obstacle;
    (obstacle_avoidance -> collision_detector).walls = walls;


    cout << "Hice evitador de obstaculos" << endl;

    /******************************  DESCANSAR *******************/

    // Hacer Arrive
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

    // Hacer priority steering
    PrioritySteering *priority_steering_rest    = new PrioritySteering();
    (priority_steering_rest -> behaviours).resize(2);
    (priority_steering_rest -> behaviours)[0] = obstacle_avoidance;
    (priority_steering_rest -> behaviours)[1] = arrive;
    priority_steering_rest -> character = &(vigilant -> character);
    priority_steering_rest -> epsilon   = epsilon;

    cout << "Arrive para descansar" << endl;

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

    cout << "Seek para vigilar en la zona" << endl;

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


    /************************ ACCIONES DE TRANSICIONES *****************/
    FindNode *find_node = new FindNode();
    find_node -> seek = seek;
    find_node -> node = node;
    find_node -> graph = graph;


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


    /********************************* MAQUINA DE ESTADOS **********************/
    (vigilant -> state_machine).states.resize(5);
    State &initial_state    = (vigilant -> state_machine).states[0];
    State &rest             = (vigilant -> state_machine).states[1];
    State &guard            = (vigilant -> state_machine).states[2];

    (vigilant -> state_machine).initial_state = &initial_state;
    (vigilant -> state_machine).current_state = &initial_state;

    /************************** ESTADOS *******************************/

    initial_state.action    = none;
    rest.action             = resting;
    guard.action            = random_moving;

    initial_state.transitions.push_back({&guard, always_true, find_node});
    rest.transitions.push_back({&guard, random_rest_guard, none});
    guard.transitions.push_back({&rest, random_guard_rest, none});
    cout << "maquina de estados" << endl;
}