#include <iostream>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

void replace(std::string& str, const std::string from, std::string  to, int key)
{
	switch (key)
	{
	case 1: to = to.substr(1, 10); break;
	case 2: to = to.substr(1, 3); break;
	}
	int start_pos = str.find(from);
	if (start_pos == std::string::npos) return;

	str.replace(start_pos, from.length(), to);
}

//APPID=b576bad9c62faf9d5714c2d9773fa2b7

void gen_response(const httplib::Request& req, httplib::Response& res)
{
	std::string HtmlCode;
	httplib::Client cli("api.openweathermap.org", 80);
	auto result = cli.Get("/data/2.5/forecast?id=688533&units=metric&APPID=b576bad9c62faf9d5714c2d9773fa2b7");
	json j;
	if (result && result->status == 200) j = json::parse(result->body);

	std::ifstream stream("Weather widget.html");
	getline(stream, HtmlCode, '\0');
	stream.close();

	replace(HtmlCode, "{city.name}", j["city"]["name"].dump(), 1);
	for (int i = 0; i < 5; i++)
	{

		int temp = j["list"][0]["dt"];
		for (int i = 0; i < 40; i++) 
		{
			if (j["list"][i]["dt"] >= temp)
			{
				replace(HtmlCode, "{list.dt}", j["list"][i]["dt_txt"].dump(), 1);
				replace(HtmlCode, "{list.weather.icon}", j["list"][i]["weather"][0]["icon"].dump(), 2);
				replace(HtmlCode, "{list.main.temp}", j["list"][i]["main"]["temp"].dump(), 0);
				temp += 86400;
			}
		}
	}

	res.set_content(HtmlCode, "html");
}


int main()
{
	httplib::Server svr;
	svr.Get("/", gen_response);
	svr.listen("localhost", 3000);
}