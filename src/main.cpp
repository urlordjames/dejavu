#include <drogon/HttpAppFramework.h>
#include <filesystem>

int main() {
	drogon::app().setDocumentRoot(STATIC_ROOT);

	std::filesystem::path upload_path = std::filesystem::temp_directory_path() / "dejavu_temp";
	// explicit string conversion required, doesn't compile on Windows otherwise
	drogon::app().setUploadPath(upload_path.string());

	// set file types drogon will send
	std::vector<std::string> types({"html", "js", "ogg"});
	drogon::app().setFileTypes(types);

	drogon::app().addListener("0.0.0.0", 8080).run();
}
