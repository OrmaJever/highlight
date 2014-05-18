#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>

#include "data.h"
#include <sstream>
#include <fstream>
#include <chrono>

typedef HL::Data::string string;
typedef std::chrono::high_resolution_clock chrono;

class HighLight : virtual public fastcgi::Component, virtual public fastcgi::Handler
{
public:
	HighLight(fastcgi::ComponentContext *context) : fastcgi::Component(context){}

	virtual void onLoad() {}
	virtual void onUnload(){}

	virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context)
	{
		request->setContentType("text/html");
		string result, content;
		if(request->getRequestMethod() == "POST" && request->hasArg("code")) {
			auto start = chrono::now();
			auto code = HL::Data::highlight(static_cast< string >(request->getArg("code")), 0);
			auto end = chrono::now();
			auto work_time = std::chrono::duration_cast< std::chrono::milliseconds >( end - start ).count();

			result << std::to_string(work_time) << "ms (";
			result << std::to_string(code.size()) << " length)";
			result << "<div class='code'><ol><li>";
			result << code.replace("<br/>", static_cast< string >("</li><li>"));
			result << "</li></ol></div>";
			HL::Data::free();
		}
		std::fstream file("/etc/fastcgi2/available/index.htm");
		if(!file) {
			std::stringbuf buf("cannot open tpl file");
			request->write(&buf);
		};

		std::ostringstream oss;
		oss << file.rdbuf();
		file.close();
		content = oss.str();
		std::stringbuf buffer(content.replace("{result}", result));
		request->write(&buffer);
	}
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("HighLightFactory", HighLight)
FCGIDAEMON_REGISTER_FACTORIES_END()
