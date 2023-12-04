#include <includes.hpp>
#pragma comment(lib, "libcurl.lib")

#define CURL_STATICLIB 

struct channel_struct
{
	std::string author;
	std::string message;
	std::string timestamp;
};

namespace KeyAuth {
	class api {
	public:

		std::string name, ownerid, secret, version, url;
		api(std::string name, std::string ownerid, std::string secret, std::string version, std::string url) : name(name), ownerid(ownerid), secret(secret), version(version), url(url) {}
		void ban(std::string reason = "");
		void init();
		void check();
		void log(std::string msg);
		void license(std::string key);
		std::string var(std::string varid);
		std::string webhook(std::string id, std::string params, std::string body = "", std::string contenttype = "");
		void setvar(std::string var, std::string vardata);
		std::string getvar(std::string var);
		bool checkblack();
		void web_login();
		void button(std::string value);
		void upgrade(std::string username, std::string key);
		void login(std::string username, std::string password);
		std::vector<unsigned char> download(std::string fileid);
		void regstr(std::string username, std::string password, std::string key, std::string email = "");
		void chatget(std::string channel);
		bool chatsend(std::string message, std::string channel);
		void changeusername(std::string newusername);
		std::string fetchonline();
		void fetchstats();
		void forgot(std::string username, std::string email);

		class subscriptions_class {
		public:
			std::string name;
			std::string expiry;
		};

		class data_class {
		public:
			// app data
			std::string numUsers;
			std::string numOnlineUsers;
			std::string numKeys;
			std::string version;
			std::string customerPanelLink;
			// user data
			std::string username;
			std::string ip;
			std::string hwid;
			std::string createdate;
			std::string lastlogin;

			std::vector<subscriptions_class> subscriptions;

			// response data
			std::vector<channel_struct> channeldata;
			bool success{};
			std::string message;
		};
		data_class data;
	private:
		std::string sessionid, enckey;

		static std::string req(std::string data, std::string url);

		void load_user_data(nlohmann::json data) {
			api::data.username = data[(skCrypt("username").decrypt())];
			api::data.ip = data[(skCrypt("ip").decrypt())];
			if (data[(skCrypt("hwid").decrypt())].is_null()) {
				api::data.hwid = skCrypt("none").decrypt();
			}
			else {
				api::data.hwid = data[(skCrypt("hwid").decrypt())];
			}
			api::data.createdate = data[(skCrypt("createdate").decrypt())];
			api::data.lastlogin = data[(skCrypt("lastlogin").decrypt())];

			for (int i = 0; i < data[(skCrypt("subscriptions").decrypt())].size(); i++) { // Prompto#7895 & stars#2297 was here
				subscriptions_class subscriptions;
				subscriptions.name = data[(skCrypt("subscriptions").decrypt())][i][(skCrypt("subscription").decrypt())];
				subscriptions.expiry = data[(skCrypt("subscriptions").decrypt())][i][(skCrypt("expiry").decrypt())];
				api::data.subscriptions.emplace_back(subscriptions);
			}
		}

		void load_app_data(nlohmann::json data) {
			api::data.numUsers = data[(skCrypt("numUsers").decrypt())];
			api::data.numOnlineUsers = data[(skCrypt("numOnlineUsers").decrypt())];
			api::data.numKeys = data[(skCrypt("numKeys").decrypt())];
			api::data.version = data[(skCrypt("version").decrypt())];
			api::data.customerPanelLink = data[(skCrypt("customerPanelLink").decrypt())];
		}

		void load_response_data(nlohmann::json data) {
			api::data.success = data[(skCrypt("success").decrypt())];
			api::data.message = data[(skCrypt("message").decrypt())];
		}

		void load_channel_data(nlohmann::json data) {
			api::data.success = data[(skCrypt("success").decrypt())];
			api::data.message = data[(skCrypt("message").decrypt())];
			for (auto sub : data[(skCrypt("messages").decrypt())])
			{
				std::string authoroutput = sub[(skCrypt("author").decrypt())];
				std::string messageoutput = sub[(skCrypt("message").decrypt())];
				std::string timestampoutput = sub[(skCrypt("timestamp").decrypt())];
				authoroutput.erase(remove(authoroutput.begin(), authoroutput.end(), '"'), authoroutput.end());
				messageoutput.erase(remove(messageoutput.begin(), messageoutput.end(), '"'), messageoutput.end());
				timestampoutput.erase(remove(timestampoutput.begin(), timestampoutput.end(), '"'), timestampoutput.end());
				channel_struct output = { authoroutput , messageoutput, timestampoutput };
				api::data.channeldata.push_back(output);
			}
		}

		nlohmann::json response_decoder;

	};
}
