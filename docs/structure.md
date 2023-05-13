# Structure
## Main class
The main class of the project is TicketSystem, with all the interfaces needed in the management system. To be more specific, the definition of the class is given below.

```cpp
class TicketSystem {
 public:
  TicketSystem(const std::string &file_name, bool inherit_file = true);
  ~TicketSystem();

  /* and other operations below */
 private:
  UserSystem user_sys_;
  TrainSystem train_sys_;
};
```

For the operation add_user, which is given by
```
[<timestamp>] add_user -<key> <argument> ...
  -c <cur_username> Current username, maximum length 20
  -u <username>     New username    , maximum length 20
  -p <password>     New password    , maximum length 30
  -n <name>         New name        , maximum length 20
  -m <mailAddr>     Email address   , maximum length 30
  -g <privilege>    Privilege       , integer in [0, 10]  
```
it provides function
```cpp
void AddUser(int timestamp, const std::string &cur_username, const std::string &username, const std::string &passwd, const std::string &name, const std::string &mail_addr, int priv);
```
which accesses the class UserSystem with similar arguments passed, and then output the return value.

For the operation login, which is given by
```
[<timestamp>] login -<key> <argument> ...
  -u <username>  Username      , maximum length 20
  -p <password>  User password , maximum length 30
```
it provides function
```cpp
void Login(int timestamp, const std::string &username, const std::string &passwd);
```
which accesses the class UserSystem with similar arguments passed, and then output the return value.

For the operation logout, which is given by
```
[<timestamp>] logout -u <username>
```
it provides function
```cpp
void Logout(int timestamp, const std::string &username);
```
which accesses the class UserSystem with similar arguments passed, and then output the return value.

For the operation query_profile, which is given by
```
[<timestamp>] query_profile -<key> <argument> ...
  -c <cur_username> Current username, maximum length 20
  -u <username>     Target username , maximum length 20
```
it provides function
```cpp
void QueryProfile(int timestamp, const std::string &cur_username, const std::string &username);
```
which accesses the class UserSystem with similar arguments passed, and then output the return message.

For the operation modify_profile, which is given by
```
[<timestamp>] add_user -<key> <argument> ...
  -c <cur_username> Current username, maximum length 20
  -u <username>     New username    , maximum length 20
 Optional:
  -p <password>     New password    , maximum length 30
  -n <name>         New name        , maximum length 20
  -m <mailAddr>     Email address   , maximum length 30
  -g <privilege>    Privilege       , integer in [0, 10]  
```
it provides function
```cpp
void ModifyProfile(int timestamp, const std::string &cur_username, const std::string &username, const std::string &passwd, const std::string &name, const std::string &mail_addr, int priv);
```
it sets the unused arguments to empty or -1 to access the class UserSystem, and then output the return message.

For the operation add_train, which is given by
```
[<timestamp>] add_train -<key> <argument> ...
  -i <trainID>       New train ID, maximum length 20
  -n <stationNum>    Number of stations
  -m <seatNum>       Number of seats
  -s <stations>      <stationNum> of Stations ID, maximum length 30*100
  -p <prices>        <stationNum-1> of Prices
  -x <startTime>     Start time
  -t <travelTimes>   <stationNum-1> of travel times
  -o <stopoverTimes> <stationNum-2> of stop over times
  -d <saleDate>      Range of sale date, given by mm-dd|mm-dd
  -y <type>          Train type, in a single char
```
it provides function
```cpp
void AddTrain(int timestamp, const std::string &train_id, int station_num, const vector<std::string> &stations, const vector<int> &prices, std::string start_time, const vector<int> &travel_time, const vector<int> &stopover_time, const std::string &saledate_start, const std::string &saledate_end, char type);
```
it converts the input time to the arguments that TrainSystem requires, and then passed them to the same function in that class.

For the operation delete_train, which is given by
```
[<timestamp>] delete_train -i <trainID>
```
it provides function
```cpp
void DeleteTrain(int timestamp, const std::string &train_id);
```
which accesses the class TrainSystem with similar arguments passed, and then output the return value.

For the operation release_train, which is given by
```
[<timestamp>] release_train -i <trainID>
```
it provides function
```cpp
void ReleaseTrain(int timestamp, const std::string &train_id);
```
which accesses the class TrainSystem with similar arguments passed, and then output the return value.

For the operation query_train, which is given by
```
[<timestamp>] query_train -<key> <argument> ...
  -i <trainID> Train ID     , maximum length 20
  -d <date>    Date to query, given by mm-dd
```
it provides function
```cpp
void QueryTrain(int timestamp, const std::string &train_id, const std::string &date);
```
which accesses the class TrainSystem with similar arguments passed, and then output the return value.

For the operation query_ticket and query_transfer, which is given by
```
[<timestamp>] query_ticket / query_transfer -<key> <argument> ...
  -s <start>       The start station      , maximum length 30
  -t <dest>        The destination station, maximum length 30
  -d <date>        Date to query          , given by mm-dd
  -p [time / cost]
```
it provides function
```cpp
void QueryTicket(const std::string &start, const std::string &dest, const std::string &date, const std::string &sort_tag);
void QueryTransfer(const std::string &start, const std::string &dest, const std::string &date, const std::string &sort_tag);
```
which accesses the class TrainSystem with similar arguments passed, and then output the return value.

For the operation buy_ticket, which is given by
```
[<timestamp>] buy_ticket -<key> <argument> ...
  -u <username>  Username               , maximum length 20
  -i <trainID>   Train ID               , maximum length 20
  -d <date>      Date to buy            , given by mm-dd
  -f <start>     The start station      , maximum length 30
  -t <dest>      The destination station, maximum length 30
  -q [true / false]
```
it provides function
```cpp
void BuyTicket(const std::string &user_name, const std::string &train_id, const std::string &date, const std::string &start, const std::string &dest);
```
which access the class UserSystem first to check the login status, get the ticket_count property, create an order and passed it into class TrainSystem. Finally, if the return value is not "-1", update the order_count property again.

For the operation refund_ticket, which is given by
```
[<timestamp>] refund_ticket -<key> <argument> ..
  -u <username>  Username           , maximum length 20
  -n <count>     Count of the ticket, 1-base
```
it procides function
```cpp
void RefundTicket(const std::string &user_name, int ticket_count);
```
which accesses the class TrainSystem with similar arguments passed, and then output the return value.

## Operation parts
The main operations are performed by the two classes below.

### User information
The basic types of user information is listed:
```cpp
using UserName = StringKey<20>;
using UserNickname = StringKey<20>;
using UserPassword = StringKey<30>;
using UserEmail = StringKey<30>;
using UserPrivilege = int;

struct UserInfo {
  UserName user_name_;
  UserNickname user_nickname_;
  UserPassword password_;
  UserEmail email_;
  UserPrivilege privilege_;
  bool login_status_{false};
  int ticket_count_{0};
};
```
The class UserSystem manages the user information, with definition

```cpp
class UserSystem {
 public:
  UserSystem(const std::string &file_name, bool inherit_file = true);
  ~UserSystem();

  /* and other operations below */

 private:
  BPlusTreeIndex<UserName, UserInfo> user_info_db_;
};
```

As is mentioned above, it provide functions only used by TicketSystem.

The function AddUser() insert a data into database after checking if the current user is valid.
```cpp
auto AddUser(const UserName &cur_username, const UserName &username, const UserPassword &passwd, const UserNickname &nickname, const UserEmail &mail_addr, const UserPrivilege &priv) -> bool;
```
it returns false if register failed (i.e. username existed or privilege invalid), true otherwise.

The function Login() uses the index_iterator of database to check and update the status of a user.
```cpp
auto Login(const UserName &username, const UserPassword &passwd) -> bool;
```
it returns false if login failed (i.e. username not existed or passwd wrong), true otherwise.

The function Logout() performs the similar operation as the function above.
```cpp
auto Logout(const UserName &username) -> bool;
```
it returns false if logout failed (i.e. username not existed or not logined), true otherwise.

The function QueryProfile() uses the search function of database to get the info of users.
```cpp
auto QueryProfile(const UserName &cur_username, const UserName &username) -> std::string;
```
it return the output information if query succeed, "-1" otherwise (i.e. username not existed or privilege invaild).

The function ModifyProfile() provides the similar operation as register.
```cpp
auto ModifyProfile(const UserName &cur_username, const UserName &username, const UserPassword &passwd, const UserNickname &nickname, const UserEmail &mail_addr, const UserPrivilege &priv) -> std::string;
```
Sometimes you may not use some of the arguments. In such case you can simply set them to empty (priv to -1).

The function BuyNewTicket() and BuyNewTicketFailed() check the login status and increase ticket_count_.
```cpp
auto BuyNewTicket(const UserName &username) -> int;
void BuyNewTicketFailed(const UserName &username);
```
First one would return -1 if the user isn't logined.

### Train Information
The basic types of train information is listed:
```cpp
using TrainID = StringKey<20>;
using StationID = StringKey<30>;
using TrainDateID = PairKey<TrainID, int>;
using TrainStationID = PairKey<StationID, TrainID>;
using TicketID = PairKey<UserName, int>;

constexpr int STATION_NUM_MAX = 100;
constexpr int WAITLIST_LENGTH = 100;

struct TrainInfo { // About 4500 bytes
  TrainID train_id_;
  int seat_num_;
  int station_num_;

  StationID station_id_[STATION_NUM_MAX];
  int prices_[STATION_NUM_MAX];
  int dep_time_[STATION_NUM_MAX];
  int arr_time_[STATION_NUM_MAX];
  
  int start_date_;
  int end_date_;
  char type_;
};

struct TrainDateInfo {
  TrainID train_id_;
  int date_;
  int remain_ticket_[STATION_NUM_MAX];

  TicketID waitlist_[WAITLIST_LENGTH];
  int head_{0}, tail_{-1};
};

struct TrainStationInfo {
  TrainID train_id_;
  StationID station_id_;
  int index_in_train_;
};

struct TicketInfo {
  TicketID ticket_id_;
  bool status_; // false for still waiting
  TrainID train_id_;
  int date_;
  int quantity_;
  int price_;
};

class TrainSystem {
 public:
  TrainSystem(const std::string &file_name, bool inherit_file = true);
  ~TrainSystem();

  /* and other operations below */

 private:
  BPlusTreeIndex<TrainID, TrainInfo> train_info_db_;
  BPlusTreeIndex<TrainDateID, TrainDateInfo> train_date_info_db_;
  BPlusTreeIndex<TrainStationID, TrainStationInfo> train_station_info_db_;
  BPlusTreeIndex<TicketID, TicketInfo> ticket_info_db_;
};
```

As is mentioned above, it provides functions used only by TicketSystem.

The function AddTrain() insert a data only into train_info_db since it's unreleased.
```cpp
auto AddTrain(const TrainID &train_id, int station_num, int seat_num, const vector<StationID> &stations, const vector<int> &prices, const vector<int> &dep_time, const vector<int> &arr_time, int start_date, int end_date, char type) -> bool;
```
it returns false if add failed (i.e. train_id exists), true otherwise.

The function DeleteTrain() removes an unreleased train from database.
```cpp
auto DeleteTrain(const TrainID &train_id) -> bool;
```
it returns false if delete failed (i.e. train_id not existed or released), true otherwise.

The function ReleaseTrain() releases an unreleased train, insert it to the two other associated database.
```cpp
auto ReleasedTrain(const TrainID &train_id) -> bool;
```
it returns false if released failed (i.e. released or not existed), true otherwise.

The function QueryTrain() access train_info database and train_date_info database (if released) and output all the data.
```cpp
auto QueryTrain(const TrainID &train_id, int date) -> std::string;
```
it returns the output information if query succeeds, "-1" otherwise (i.e. train_id not existed or not available that day).

The function QueryTicket performs such operation:
```cpp
auto QueryTicket(int date, const StationID &start, const StationID &dest, int sort_tag) -> std::string;
```
1. Access train_station_info database and get all the TrainStationInfo associated with the start and dest.
2. Merge them to get their intersection and time.
3. Operations of TrainInfo and TrainDateInfo.
4. Return the result.
5. The total time cost is (trains num of a station) * (indexing time)

It returns "0" if station_id not existed.

The function QueryTransfer() performs such operation:
```cpp
auto QueryTransfer(int date, const StationID &start, const StationID &dest, int sort_tag) -> std::string;
```
1. Access train_station_info database and get all TrainStationInfo associated with the start and dest.
2. For each train via the start and each train via the dest, get the intersection of their stations, check if time is valid.
3. Calculate the best plan for each (train1, start, train2, dest, mid).
4. Return the result.
5. The total time cost is (trains num1) * (trains num2) * (stations num) * (indexing time)

It returns "0" if station_id not existed.

The function BuyTicket performs such operation:
```cpp
auto BuyTicket(const TicketInfo &ticket, bool wait_tag) -> std::string;
```
1. Access train_info database and get the price.
2. Access train_date_info database and get the waiting status.
3. If it should be added into waitlist, update the TrainDateInfo at waitlist.
4. Or update the TrainDateInfo at remain_ticket.

It returns the total price if succeeds, "queue" if waitlisted and "-1" if failed.

The function QueryOrder access ticket_info database and find the assoicated orders.
```cpp
auto QueryOrder(const UserName &username) -> std::string;
```
It returns "" if not found order.

The function RefundTicket performs such operation:
```cpp
auto RefundTicket(const UserName &username, int order_num);
```
1. Access ticket_info database, find corresponding order and edit its status.
2. Access train_data_info database and edit the remain_ticket_ or waitlist.
3. Scan the waitlist.