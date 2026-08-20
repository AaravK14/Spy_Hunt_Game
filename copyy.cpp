#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <memory>
#include <map>
#include <sstream>
#include <ctime>
#include <chrono>


#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #define closesocket close
#endif

using namespace std;
using namespace std::chrono;



class GameObserver;
class GameSubject;

class GameEntity {
protected:  
    int id;
    string name;
    bool isActive;
    
public:
    GameEntity(int entityId, const string& entityName) 
        : id(entityId), name(entityName), isActive(true) {}
    
    virtual ~GameEntity() = default;
    virtual string getRole() const = 0;
    virtual void displayInfo() const = 0;
    
    int getId() const { return id; }
    string getName() const { return name; }
    bool getIsActive() const { return isActive; }
    void setActive(bool active) { isActive = active; }
    
    bool performAction(const string& action, const string& data = "") {
        if (!isActive) return false;
        return executeAction(action, data);
    }
    
protected:
    virtual bool executeAction(const string& action, const string& data) = 0;
};

class Player : public GameEntity {  
protected: 
    string word;
    vector<string> hintsGiven;
    int votesReceived;
    bool hasVoted;
    
public:
    Player(int playerId, const string& playerName) 
        : GameEntity(playerId, playerName), votesReceived(0), hasVoted(false) {}
    
    virtual ~Player() override = default;
    
    string getRole() const override {
        return "Player";
    }
    
    void displayInfo() const override {
        cout << "Player: " << name << " (ID: " << id << ")" << endl;
    }
    
    void setWord(const string& playerWord) { word = playerWord; }
    string getWord() const { return word; }
    
    void addHint(const string& hint) { hintsGiven.push_back(hint); }
    vector<string> getHints() const { return hintsGiven; }
    
    void addVote() { votesReceived++; }
    int getVotesReceived() const { return votesReceived; }
    void resetVotes() { votesReceived = 0; }
    
    void setVoted(bool voted) { hasVoted = voted; }
    bool getHasVoted() const { return hasVoted; }
    
    virtual double getSuspicionLevel() const {
        return hintsGiven.size() * 0.1; 
    }
    
protected:
    bool executeAction(const string& action, const string& data) override {   
        if (action == "hint") {
            if (!data.empty()) {
                addHint(data);
                return true;
            }
        } else if (action == "vote") {
            if (!hasVoted) {
                hasVoted = true;
                return true;
            }
        }
        return false;
    }
};

class Spy : public Player {  
private: 
    string spyWord;
    int suspicionLevel;
    bool identityRevealed;
    
public:
    Spy(int spyId, const string& spyName) 
        : Player(spyId, spyName), suspicionLevel(0), identityRevealed(false) {}
    
    string getRole() const override {
        return "Spy";
    }
    
    void displayInfo() const override {
        cout << "SPY: " << name << " (ID: " << id << ")" << endl;
    }
    
    void setSpyWord(const string& word) { spyWord = word; }
    string getSpyWord() const { return spyWord; }
    
    void increaseSuspicion() { suspicionLevel++; }
    int getSuspicionRating() const { return suspicionLevel; }
    
    void revealIdentity() { identityRevealed = true; }
    bool isIdentityRevealed() const { return identityRevealed; }
    
    double getSuspicionLevel() const override {
        return (hintsGiven.size() * 0.3) + (suspicionLevel * 0.5);
    }
    
protected:
    bool executeAction(const string& action, const string& data) override {
        bool result = Player::executeAction(action, data);
        if (result && action == "hint") {
            increaseSuspicion();
        }
        return result;
    }
};

struct WordPair {
    string normalWord;
    string spyWord;
    string theme;
    
    WordPair(const string& normal = "", const string& spy = "", const string& category = "")
        : normalWord(normal), spyWord(spy), theme(category) {}
};

class WordManager {
private: 
    map<string, vector<WordPair>> wordDatabase;
    mutable mt19937 randomEngine;  
    
    void initializeDatabase() {
        wordDatabase["animals"] = {
            WordPair("Dog", "Wolf", "animals"),
            WordPair("Cat", "Lion", "animals"),
            WordPair("Horse", "Zebra", "animals"),
            WordPair("Chicken", "Eagle", "animals"),
            WordPair("Fish", "Shark", "animals"),
            WordPair("Cow", "Buffalo", "animals"),
            WordPair("Rabbit", "Hare", "animals")
        };
        
        wordDatabase["food"] = {
            WordPair("Pizza", "Burger", "food"),
            WordPair("Apple", "Orange", "food"),
            WordPair("Bread", "Cake", "food"),
            WordPair("Coffee", "Tea", "food"),
            WordPair("Rice", "Pasta", "food"),
            WordPair("Ice Cream", "Yogurt", "food"),
            WordPair("Soup", "Stew", "food")
        };
        
        wordDatabase["places"] = {
            WordPair("Beach", "Desert", "places"),
            WordPair("School", "Office", "places"),
            WordPair("Hospital", "Clinic", "places"),
            WordPair("Park", "Garden", "places"),
            WordPair("Library", "Bookstore", "places"),
            WordPair("Restaurant", "Cafe", "places"),
            WordPair("Mall", "Market", "places")
        };
        
        wordDatabase["objects"] = {
            WordPair("Chair", "Table", "objects"),
            WordPair("Phone", "Computer", "objects"),
            WordPair("Car", "Motorcycle", "objects"),
            WordPair("Book", "Magazine", "objects"),
            WordPair("Pen", "Pencil", "objects"),
            WordPair("Watch", "Clock", "objects"),
            WordPair("Camera", "Telescope", "objects")
        };
    }
    
public:
    WordManager() : randomEngine(static_cast<unsigned int>(time(nullptr))) {
        initializeDatabase();
    }
    
    WordPair getRandomWordPair(const string& theme) const {
        auto it = wordDatabase.find(theme);
        if (it != wordDatabase.end() && !it->second.empty()) {
            uniform_int_distribution<size_t> dist(0, it->second.size() - 1);
            return it->second[dist(randomEngine)];
        }
        uniform_int_distribution<size_t> dist(0, wordDatabase.at("animals").size() - 1);
        return wordDatabase.at("animals")[dist(randomEngine)];
    }
    
    vector<string> getAvailableThemes() const {
        vector<string> themes;
        for (const auto& pair : wordDatabase) {
            themes.push_back(pair.first);
        }
        return themes;
    }
};

class GameTimer {
private:
    steady_clock::time_point startTime;
    int durationSeconds;
    bool isRunning;
    
public:
    GameTimer() : durationSeconds(0), isRunning(false) {}
    
    void start(int seconds) {
        startTime = steady_clock::now();
        durationSeconds = seconds;
        isRunning = true;
    }
    
    void stop() {
        isRunning = false;
    }
    
    int getRemainingTime() const {
        if (!isRunning) return 0;
        
        auto currentTime = steady_clock::now();
        auto elapsed = duration_cast<seconds>(currentTime - startTime).count();
        int remaining = durationSeconds - static_cast<int>(elapsed);
        
        return max(0, remaining);
    }
    
    bool hasExpired() const {
        return isRunning && getRemainingTime() <= 0;
    }
    
    bool isActive() const {
        return isRunning;
    }
};

class SpyHuntGameEngine {
private:
    vector<unique_ptr<Player>> players;
    Spy* currentSpy;
    WordManager wordManager;
    WordPair currentWords;
    int totalPlayers;
    int spyIndex;
    int totalGames;
    int spyWins;
    int playerWins;
    
    GameTimer wordRevealTimer;
    GameTimer hintPhaseTimer;
    GameTimer discussionTimer;
    GameTimer votingTimer;
    
    string currentPhase;            // "wordReveal", "hint", "discussion", "voting", "results"
    
public:
    SpyHuntGameEngine() : currentSpy(nullptr), totalPlayers(0), spyIndex(-1), 
currentPhase("idle") {}

string startGame(int numPlayers, const string& theme) {
        players.clear();
totalPlayers = numPlayers;

for (int i = 1; i <= numPlayers; ++i) {
            players.push_back(make_unique<Player>(i, "Player " + to_string(i)));
}

mt19937 rng(static_cast<unsigned int>(time(nullptr)));
        uniform_int_distribution<int> dist(0, numPlayers - 1);
        spyIndex = dist(rng);
        
        int spyId = players[spyIndex]->getId();
        string spyName = players[spyIndex]->getName();
        
        auto spy = make_unique<Spy>(spyId, spyName);
        currentSpy = spy.get();
players[spyIndex] = move(spy);

currentWords = wordManager.getRandomWordPair(theme);
        
        for (auto& player : players) {
            if (player->getRole() == "Spy") {
                player->setWord(currentWords.spyWord);
                static_cast<Spy*>(player.get())->setSpyWord(currentWords.spyWord);
            } else {
                player->setWord(currentWords.normalWord);
            }
}

currentPhase = "wordReveal";
        wordRevealTimer.start(10);
        
return "{\"success\":true,\"spyIndex\":" + to_string(spyIndex) + ",\"phase\":\"wordReveal\",\"timeLimit\":10}";
}

string getPlayerWord(int playerIndex) {
        if (playerIndex < 0 || playerIndex >= (int)players.size()) {
            return "{\"error\":\"Invalid player index\"}";
        }
        
        string role = players[playerIndex]->getRole();
        string word = players[playerIndex]->getWord();
        bool isSpy = (playerIndex == spyIndex);
        
        stringstream ss;
        ss << "{\"word\":\"" << word << "\",\"role\":\"" << role 
           << "\",\"isSpy\":" << (isSpy ? "true" : "false") << "}";
        
return ss.str();
}

string startHintPhase() {
currentPhase = "hint";
hintPhaseTimer.start(30);
return "{\"success\":true,\"phase\":\"hint\",\"timeLimit\":30}";
    }
    
    string submitHint(int playerIndex, const string& hint) {
        if (playerIndex < 0 || playerIndex >= (int)players.size()) {
return "{\"error\":\"Invalid player index\"}";
}

if (currentPhase != "hint") {
            return "{\"error\":\"Not in hint phase\"}";
}

if (hintPhaseTimer.hasExpired()) {
            return "{\"error\":\"Time expired\"}";
        }
        
        players[playerIndex]->performAction("hint", hint);
return "{\"success\":true}";
}

string startDiscussionPhase() {
currentPhase = "discussion";
discussionTimer.start(60);
return "{\"success\":true,\"phase\":\"discussion\",\"timeLimit\":60}";
    }
    
    string getAllHints() {
        stringstream ss;
        ss << "{\"hints\":[";
        
        for (size_t i = 0; i < players.size(); ++i) {
            const auto& hints = players[i]->getHints();
            ss << "{\"player\":\"" << players[i]->getName() << "\",";
            ss << "\"hint\":\"" << (hints.empty() ? "" : hints[0]) << "\"}";
            if (i < players.size() - 1) ss << ",";
        }
        
        ss << "]}";
return ss.str();
}

string startVotingPhase() {
currentPhase = "voting";
votingTimer.start(20);
return "{\"success\":true,\"phase\":\"voting\",\"timeLimit\":20}";
    }
    
    string submitVote(int votedPlayerIndex) {
        if (votedPlayerIndex < 0 || votedPlayerIndex >= (int)players.size()) {
return "{\"error\":\"Invalid player index\"}";
}

if (currentPhase != "voting") {
            return "{\"error\":\"Not in voting phase\"}";
}

if (votingTimer.hasExpired()) {
            return "{\"error\":\"Time expired\"}";
}

players[votedPlayerIndex]->addVote();

mt19937 rng(static_cast<unsigned int>(time(nullptr)));
        uniform_int_distribution<int> dist(0, static_cast<int>(players.size()) - 1);
        
        for (int i = 0; i < totalPlayers - 1; ++i) {
            int voteTarget = dist(rng);
            players[voteTarget]->addVote();
}

Player* mostVoted = players[0].get();
        for (const auto& player : players) {
            if (player->getVotesReceived() > mostVoted->getVotesReceived()) {
                mostVoted = player.get();
            }
        }
        
        bool spyWon = (mostVoted != currentSpy);

        totalGames++;
        if (spyWon) {
            spyWins++;
        } else {
playerWins++;
}

currentPhase = "results";
votingTimer.stop();

stringstream ss;
        ss << "{\"spyWon\":" << (spyWon ? "true" : "false") << ",";
        ss << "\"spyIndex\":" << spyIndex << ",";
        ss << "\"spyName\":\"" << currentSpy->getName() << "\",";
        ss << "\"normalWord\":\"" << currentWords.normalWord << "\",";
        ss << "\"spyWord\":\"" << currentWords.spyWord << "\",";
        ss << "\"mostVotedName\":\"" << mostVoted->getName() << "\",";
        ss << "\"phase\":\"results\",";
        ss << "\"votes\":[";
        
        for (size_t i = 0; i < players.size(); ++i) {
            ss << players[i]->getVotesReceived();
            if (i < players.size() - 1) ss << ",";
        }
        
ss << "]}";

for (auto& player : players) {
            player->resetVotes();
            player->setVoted(false);
        }
        
return ss.str();
}

string getGameStatus() {
        stringstream ss;
        ss << "{\"phase\":\"" << currentPhase << "\",";
        
        int remainingTime = 0;
        if (currentPhase == "wordReveal") {
            remainingTime = wordRevealTimer.getRemainingTime();
        } else if (currentPhase == "hint") {
            remainingTime = hintPhaseTimer.getRemainingTime();
        } else if (currentPhase == "discussion") {
            remainingTime = discussionTimer.getRemainingTime();
        } else if (currentPhase == "voting") {
            remainingTime = votingTimer.getRemainingTime();
        }
        
        ss << "\"remainingTime\":" << remainingTime << ",";
        ss << "\"timeExpired\":" << (remainingTime <= 0 && currentPhase != "idle" && currentPhase != "results" ? "true" : "false");
        ss << "}";
        
return ss.str();
}

string getStats() {
        stringstream ss;
        ss << "{\"totalGames\":" << totalGames << ",";
        ss << "\"spyWins\":" << spyWins << ",";
        ss << "\"playerWins\":" << playerWins << "}";
        return ss.str();
    }
};

class SimpleHTTPServer {
private:
    int serverSocket;
    SpyHuntGameEngine gameEngine;
    
    string urlDecode(const string& str) {
        string result;
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == '%' && i + 2 < str.length()) {
                int value;
                sscanf(str.substr(i + 1, 2).c_str(), "%x", &value);
                result += static_cast<char>(value);
                i += 2;
            } else if (str[i] == '+') {
                result += ' ';
            } else {
                result += str[i];
            }
        }
        return result;
    }
    
    map<string, string> parseQuery(const string& query) {
        map<string, string> params;
        stringstream ss(query);
        string pair;
        
        while (getline(ss, pair, '&')) {
            size_t pos = pair.find('=');
            if (pos != string::npos) {
                string key = urlDecode(pair.substr(0, pos));
                string value = urlDecode(pair.substr(pos + 1));
                params[key] = value;
            }
        }
        
        return params;
    }
    
    string handleRequest(const string& request) {
        stringstream ss(request);
        string method, path, version;
        ss >> method >> path >> version;
        
        string response;
        string contentType = "application/json";
        
        size_t queryPos = path.find('?');
        string endpoint = path.substr(0, queryPos);
        string query = (queryPos != string::npos) ? path.substr(queryPos + 1) : "";
        map<string, string> params = parseQuery(query);

        if (endpoint == "/api/start") {
            int numPlayers = stoi(params["players"]);
            string theme = params["theme"];
            response = gameEngine.startGame(numPlayers, theme);
        }
        else if (endpoint == "/api/getWord") {
            int playerIndex = stoi(params["index"]);
            response = gameEngine.getPlayerWord(playerIndex);
        }
        else if (endpoint == "/api/startHintPhase") {
            response = gameEngine.startHintPhase();
        }
        else if (endpoint == "/api/submitHint") {
            int playerIndex = stoi(params["index"]);
            string hint = params["hint"];
            response = gameEngine.submitHint(playerIndex, hint);
        }
        else if (endpoint == "/api/startDiscussionPhase") {
            response = gameEngine.startDiscussionPhase();
        }
        else if (endpoint == "/api/getHints") {
            response = gameEngine.getAllHints();
        }
        else if (endpoint == "/api/startVotingPhase") {
            response = gameEngine.startVotingPhase();
        }
        else if (endpoint == "/api/vote") {
            int voteIndex = stoi(params["index"]);
            response = gameEngine.submitVote(voteIndex);
        }
        else if (endpoint == "/api/status") {
            response = gameEngine.getGameStatus();
        }
        else if (endpoint == "/api/stats") {
            response = gameEngine.getStats();
        }
        else {
response = "{\"error\":\"Unknown endpoint\"}";
}

stringstream httpResponse;
        httpResponse << "HTTP/1.1 200 OK\r\n";
        httpResponse << "Content-Type: " << contentType << "\r\n";
        httpResponse << "Access-Control-Allow-Origin: *\r\n";
        httpResponse << "Content-Length: " << response.length() << "\r\n";
        httpResponse << "\r\n";
        httpResponse << response;
        
        return httpResponse.str();
    }
    
public:
    bool start(int port) {
        #ifdef _WIN32
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                cerr << "WSAStartup failed" << endl;
                return false;
            }
        #endif
        
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            cerr << "Error creating socket" << endl;
            return false;
        }
        
        int opt = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
        
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);
        
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            cerr << "Bind failed" << endl;
            closesocket(serverSocket);
            return false;
        }
        
        if (listen(serverSocket, 5) < 0) {
            cerr << "Listen failed" << endl;
            closesocket(serverSocket);
            return false;
        }
        
        cout << "==============================================\n";
        cout << "  Spy Hunt Game Server Started (WITH TIMERS)!\n";
        cout << "  Server running on: http://localhost:" << port << "\n";
        cout << "  Open spy_hunt.html in your browser\n";
        cout << "  Game Phases with Time Limits:\n";
        cout << "    - Word Reveal: 10 seconds\n";
        cout << "    - Hint Phase: 30 seconds\n";
        cout << "    - Discussion: 60 seconds\n";
        cout << "    - Voting: 20 seconds\n";
        cout << "==============================================\n";
        
        return true;
    }
    
    void run() {
        while (true) {
            sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
            
            if (clientSocket < 0) {
                continue;
            }
            
            char buffer[4096] = {0};
            recv(clientSocket, buffer, sizeof(buffer), 0);
            
            string request(buffer);
            string response = handleRequest(request);
            
            send(clientSocket, response.c_str(), response.length(), 0);
            closesocket(clientSocket);
        }
    }
    
    ~SimpleHTTPServer() {
        closesocket(serverSocket);
        #ifdef _WIN32
            WSACleanup();
        #endif
    }
};

int main() {
    SimpleHTTPServer server;
    
    if (server.start(8080)) {
        server.run();
    }
    
    return 0;
}