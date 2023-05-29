#pragma once
#include <windows.h>//wininet.hを使うのに必要
#include <wininet.h>//HTTP通信に必要
#include <sstream>


#define URLBUFFER_SIZE		(4096*4)

static bool HttpRequest(
	std::string strUserAgent,
	std::string strUrl,
	std::string strParameter,
	std::string& rstrResult,
	int Size
) {
	HINTERNET			hInternetOpen = nullptr;
	HINTERNET			hInternetConnect = nullptr;
	HINTERNET			hInternetRequest = nullptr;
	while (true) {
		// アウトプットの初期化
		rstrResult = "";

		// インプットのチェック
		if (0 == strUrl.length()) {
			assert(!"URLが不正");
			return false;
		}

		// 変数
		const char*			pszOptional = strParameter.c_str();
		URL_COMPONENTS		urlcomponents;
		std::string			strServer;
		std::string			strObject;
		INTERNET_PORT		nPort;
		std::string			strVerb;
		std::string			strHeaders;
		std::stringstream	ssRead;

		// URL解析
		ZeroMemory(&urlcomponents, sizeof(URL_COMPONENTS));
		urlcomponents.dwStructSize = sizeof(URL_COMPONENTS);
		TCHAR szHostName[URLBUFFER_SIZE];
		TCHAR szUrlPath[URLBUFFER_SIZE];
		urlcomponents.lpszHostName = szHostName;
		urlcomponents.lpszUrlPath = szUrlPath;
		urlcomponents.dwHostNameLength = URLBUFFER_SIZE;
		urlcomponents.dwUrlPathLength = URLBUFFER_SIZE;
		if (!InternetCrackUrl(strUrl.c_str(), (DWORD)strUrl.length(), 0, &urlcomponents)) {	// URLの解析に失敗
			assert(!"URL解析に失敗");
			return false;
		}
		strServer = urlcomponents.lpszHostName;
		strObject = urlcomponents.lpszUrlPath;
		nPort = urlcomponents.nPort;

		// HTTPかHTTPSかそれ以外か
		DWORD dwFlags = 0;
		if (INTERNET_SCHEME_HTTP == urlcomponents.nScheme) {	// HTTP
			dwFlags = INTERNET_FLAG_RELOAD				// 要求されたファイル、オブジェクト、またはフォルダ一覧を、キャッシュからではなく、元のサーバーから強制的にダウンロードします。
				| INTERNET_FLAG_DONT_CACHE			// 返されたエンティティをキャシュへ追加しません。
				| INTERNET_FLAG_NO_AUTO_REDIRECT;	// HTTP だけで使用され、リダイレクトが HttpSendRequest で処理されないことを指定します。
		}
		else if (INTERNET_SCHEME_HTTPS == urlcomponents.nScheme) {	// HTTPS
			dwFlags = INTERNET_FLAG_RELOAD				// 要求されたファイル、オブジェクト、またはフォルダ一覧を、キャッシュからではなく、元のサーバーから強制的にダウンロードします。
				| INTERNET_FLAG_DONT_CACHE			// 返されたエンティティをキャシュへ追加しません。
				| INTERNET_FLAG_NO_AUTO_REDIRECT	// HTTP だけで使用され、リダイレクトが HttpSendRequest で処理されないことを指定します。
				| INTERNET_FLAG_SECURE						// 安全なトランザクションを使用します。これにより、SSL/PCT を使うように変換され、HTTP 要求だけで有効です。 
				| INTERNET_FLAG_IGNORE_CERT_DATE_INVALID	// INTERNET_FLAG_IGNORE_CERT_DATE_INVALID、INTERNET_FLAG_IGNORE_CERT_CN_INVALID
				| INTERNET_FLAG_IGNORE_CERT_CN_INVALID;		// は、証明書に関する警告を無視するフラグ
		}
		else {
			assert(!"HTTPでもHTTPSでもない");
			return false;
		}

		strVerb = "POST";
		strHeaders = "Content-Type: application/json";

		// WinInetの初期化
		hInternetOpen = InternetOpen(strUserAgent.c_str(),
			INTERNET_OPEN_TYPE_PRECONFIG,
			nullptr, nullptr, 0);
		if (nullptr == hInternetOpen) {
			assert(!"WinInetの初期化に失敗");
			break;
		}

		// HTTP接続
		hInternetConnect = InternetConnect(hInternetOpen,
			strServer.c_str(),
			nPort,
			nullptr,
			nullptr,
			INTERNET_SERVICE_HTTP,
			0,
			0);
		if (nullptr == hInternetConnect) {
			assert(!"HTTP接続に失敗");
			break;
		}

		// HTTP接続を開く
		hInternetRequest = HttpOpenRequest(hInternetConnect,
			strVerb.c_str(),
			strObject.c_str(),
			nullptr,
			nullptr,
			nullptr,
			dwFlags,
			NULL);
		if (nullptr == hInternetRequest) {
			assert(!"HTTP接続を開くに失敗");
			break;
		}

		// HTTP要求送信
		if (!HttpSendRequest(
			hInternetRequest,
			strHeaders.c_str(),
			(DWORD)strHeaders.length(),
			(LPVOID)((char*)pszOptional),
			pszOptional ? (DWORD)(strlen(pszOptional) * sizeof(char)) : 0)) {
			assert(!"HTTP要求送信に失敗");
			break;
		}

		// HTTP要求に対応するステータスコードの取得
		DWORD dwStatusCode;
		DWORD dwLength = sizeof(DWORD);
		if (!HttpQueryInfo(hInternetRequest,
			HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
			&dwStatusCode,
			&dwLength,
			0)) {
			assert(!"HTTP要求に対応するステータスコードの取得に失敗");
			break;
		}
		if (HTTP_STATUS_OK != dwStatusCode) {
			assert(!"ステータスコードがOKでない");
			break;
		}

		// HTTPファイル読み込み
		char* szReadBuffer = new char[Size + 1];
		while (true) {
			DWORD dwRead = 0;
			if (!InternetReadFile(hInternetRequest, szReadBuffer, Size, &dwRead)) {
				assert(!"HTTPファイル読み込みに失敗");
				InternetCloseHandle(hInternetRequest);
				InternetCloseHandle(hInternetConnect);
				InternetCloseHandle(hInternetOpen);
				return false;
			}
			if (dwRead == 0) {
				break;
			}
			szReadBuffer[dwRead] = '\0';	// 終端文字「\0」の付加
		}

		// ストリーム文字列を、出力文字列に変換
		rstrResult = szReadBuffer;
		delete[] szReadBuffer;

		InternetCloseHandle(hInternetRequest);
		InternetCloseHandle(hInternetConnect);
		InternetCloseHandle(hInternetOpen);
		return true;
	}
	InternetCloseHandle(hInternetRequest);
	InternetCloseHandle(hInternetConnect);
	InternetCloseHandle(hInternetOpen);
	return false;
}
/*
std::wstring utf8_to_wide_cppapi(std::string const& src)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(src);
}
std::string wide_to_multi_capi(std::wstring const& src)
{
	std::size_t converted{};
	std::vector<char> dest(src.size() * sizeof(wchar_t) + 1, '\0');
	if (::_wcstombs_s_l(&converted, dest.data(), dest.size(), src.data(), _TRUNCATE, ::_create_locale(LC_ALL, "jpn")) != 0) {
		throw std::system_error{ errno, std::system_category() };
	}
	dest.resize(std::char_traits<char>::length(dest.data()));
	dest.shrink_to_fit();
	return std::string(dest.begin(), dest.end());
}
std::string utf8_to_multi_cppapi(std::string const& src)
{
	auto const wide = utf8_to_wide_cppapi(src);
	return wide_to_multi_capi(wide);
}
//*/


static bool CommonDataRequest(const char* Path, int Offset, int Limit, std::string& rstrResult) {
	std::ifstream file(Path);
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	{
		std::string target = "OFFSETVAL";					// 検索文字列
		std::string replacement = std::to_string(Offset);	// 置換文字列
		size_t pos = 0;
		while ((pos = str.find(target, pos)) != std::string::npos) {
			str.replace(pos, target.length(), replacement);
			break;
		}
	}
	{
		std::string target = "LIMITVAL";					// 検索文字列
		std::string replacement = std::to_string(Limit);	// 置換文字列
		size_t pos = 0;
		while ((pos = str.find(target, pos)) != std::string::npos) {
			str.replace(pos, target.length(), replacement);
			break;
		}
	}

	while (true) {
		auto now = str.find("\n");
		if (now != std::string::npos) {
			str = str.substr(0, now) + str.substr(now + 1);
		}
		else {
			break;
		}
	}

	return HttpRequest("EFT_Assistant", "https://api.tarkov.dev/graphql", str, rstrResult, 1024 * 256);
}
