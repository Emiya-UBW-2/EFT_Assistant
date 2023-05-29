#pragma once
#include <windows.h>//wininet.h���g���̂ɕK�v
#include <wininet.h>//HTTP�ʐM�ɕK�v
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
		// �A�E�g�v�b�g�̏�����
		rstrResult = "";

		// �C���v�b�g�̃`�F�b�N
		if (0 == strUrl.length()) {
			assert(!"URL���s��");
			return false;
		}

		// �ϐ�
		const char*			pszOptional = strParameter.c_str();
		URL_COMPONENTS		urlcomponents;
		std::string			strServer;
		std::string			strObject;
		INTERNET_PORT		nPort;
		std::string			strVerb;
		std::string			strHeaders;
		std::stringstream	ssRead;

		// URL���
		ZeroMemory(&urlcomponents, sizeof(URL_COMPONENTS));
		urlcomponents.dwStructSize = sizeof(URL_COMPONENTS);
		TCHAR szHostName[URLBUFFER_SIZE];
		TCHAR szUrlPath[URLBUFFER_SIZE];
		urlcomponents.lpszHostName = szHostName;
		urlcomponents.lpszUrlPath = szUrlPath;
		urlcomponents.dwHostNameLength = URLBUFFER_SIZE;
		urlcomponents.dwUrlPathLength = URLBUFFER_SIZE;
		if (!InternetCrackUrl(strUrl.c_str(), (DWORD)strUrl.length(), 0, &urlcomponents)) {	// URL�̉�͂Ɏ��s
			assert(!"URL��͂Ɏ��s");
			return false;
		}
		strServer = urlcomponents.lpszHostName;
		strObject = urlcomponents.lpszUrlPath;
		nPort = urlcomponents.nPort;

		// HTTP��HTTPS������ȊO��
		DWORD dwFlags = 0;
		if (INTERNET_SCHEME_HTTP == urlcomponents.nScheme) {	// HTTP
			dwFlags = INTERNET_FLAG_RELOAD				// �v�����ꂽ�t�@�C���A�I�u�W�F�N�g�A�܂��̓t�H���_�ꗗ���A�L���b�V������ł͂Ȃ��A���̃T�[�o�[���狭���I�Ƀ_�E�����[�h���܂��B
				| INTERNET_FLAG_DONT_CACHE			// �Ԃ��ꂽ�G���e�B�e�B���L���V���֒ǉ����܂���B
				| INTERNET_FLAG_NO_AUTO_REDIRECT;	// HTTP �����Ŏg�p����A���_�C���N�g�� HttpSendRequest �ŏ�������Ȃ����Ƃ��w�肵�܂��B
		}
		else if (INTERNET_SCHEME_HTTPS == urlcomponents.nScheme) {	// HTTPS
			dwFlags = INTERNET_FLAG_RELOAD				// �v�����ꂽ�t�@�C���A�I�u�W�F�N�g�A�܂��̓t�H���_�ꗗ���A�L���b�V������ł͂Ȃ��A���̃T�[�o�[���狭���I�Ƀ_�E�����[�h���܂��B
				| INTERNET_FLAG_DONT_CACHE			// �Ԃ��ꂽ�G���e�B�e�B���L���V���֒ǉ����܂���B
				| INTERNET_FLAG_NO_AUTO_REDIRECT	// HTTP �����Ŏg�p����A���_�C���N�g�� HttpSendRequest �ŏ�������Ȃ����Ƃ��w�肵�܂��B
				| INTERNET_FLAG_SECURE						// ���S�ȃg�����U�N�V�������g�p���܂��B����ɂ��ASSL/PCT ���g���悤�ɕϊ�����AHTTP �v�������ŗL���ł��B 
				| INTERNET_FLAG_IGNORE_CERT_DATE_INVALID	// INTERNET_FLAG_IGNORE_CERT_DATE_INVALID�AINTERNET_FLAG_IGNORE_CERT_CN_INVALID
				| INTERNET_FLAG_IGNORE_CERT_CN_INVALID;		// �́A�ؖ����Ɋւ���x���𖳎�����t���O
		}
		else {
			assert(!"HTTP�ł�HTTPS�ł��Ȃ�");
			return false;
		}

		strVerb = "POST";
		strHeaders = "Content-Type: application/json";

		// WinInet�̏�����
		hInternetOpen = InternetOpen(strUserAgent.c_str(),
			INTERNET_OPEN_TYPE_PRECONFIG,
			nullptr, nullptr, 0);
		if (nullptr == hInternetOpen) {
			assert(!"WinInet�̏������Ɏ��s");
			break;
		}

		// HTTP�ڑ�
		hInternetConnect = InternetConnect(hInternetOpen,
			strServer.c_str(),
			nPort,
			nullptr,
			nullptr,
			INTERNET_SERVICE_HTTP,
			0,
			0);
		if (nullptr == hInternetConnect) {
			assert(!"HTTP�ڑ��Ɏ��s");
			break;
		}

		// HTTP�ڑ����J��
		hInternetRequest = HttpOpenRequest(hInternetConnect,
			strVerb.c_str(),
			strObject.c_str(),
			nullptr,
			nullptr,
			nullptr,
			dwFlags,
			NULL);
		if (nullptr == hInternetRequest) {
			assert(!"HTTP�ڑ����J���Ɏ��s");
			break;
		}

		// HTTP�v�����M
		if (!HttpSendRequest(
			hInternetRequest,
			strHeaders.c_str(),
			(DWORD)strHeaders.length(),
			(LPVOID)((char*)pszOptional),
			pszOptional ? (DWORD)(strlen(pszOptional) * sizeof(char)) : 0)) {
			assert(!"HTTP�v�����M�Ɏ��s");
			break;
		}

		// HTTP�v���ɑΉ�����X�e�[�^�X�R�[�h�̎擾
		DWORD dwStatusCode;
		DWORD dwLength = sizeof(DWORD);
		if (!HttpQueryInfo(hInternetRequest,
			HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
			&dwStatusCode,
			&dwLength,
			0)) {
			assert(!"HTTP�v���ɑΉ�����X�e�[�^�X�R�[�h�̎擾�Ɏ��s");
			break;
		}
		if (HTTP_STATUS_OK != dwStatusCode) {
			assert(!"�X�e�[�^�X�R�[�h��OK�łȂ�");
			break;
		}

		// HTTP�t�@�C���ǂݍ���
		char* szReadBuffer = new char[Size + 1];
		while (true) {
			DWORD dwRead = 0;
			if (!InternetReadFile(hInternetRequest, szReadBuffer, Size, &dwRead)) {
				assert(!"HTTP�t�@�C���ǂݍ��݂Ɏ��s");
				InternetCloseHandle(hInternetRequest);
				InternetCloseHandle(hInternetConnect);
				InternetCloseHandle(hInternetOpen);
				return false;
			}
			if (dwRead == 0) {
				break;
			}
			szReadBuffer[dwRead] = '\0';	// �I�[�����u\0�v�̕t��
		}

		// �X�g���[����������A�o�͕�����ɕϊ�
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
		std::string target = "OFFSETVAL";					// ����������
		std::string replacement = std::to_string(Offset);	// �u��������
		size_t pos = 0;
		while ((pos = str.find(target, pos)) != std::string::npos) {
			str.replace(pos, target.length(), replacement);
			break;
		}
	}
	{
		std::string target = "LIMITVAL";					// ����������
		std::string replacement = std::to_string(Limit);	// �u��������
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
