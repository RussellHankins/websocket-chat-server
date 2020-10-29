#include "readurl.h"
#include <string.h>


// ****************************** WINDOWS *****************************
#if defined _WIN32 || defined _WIN64

The read_url function needs to be written for windows.
You can get it from this url:
https://stackoverflow.com/questions/389069/programmatically-reading-a-web-page
#endif

// ****************************** LINUX *****************************
#ifdef __unix__
#include <curl/curl.h>

stringbuilder *readurl::read_url(const char *url,const char **error_message)
{
	stringbuilder *output;
	output = new stringbuilder();
	
	CURL* curl; //our curl object
	CURLcode res; // Call result.
	*error_message = nullptr;
	
    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &readurl::write_callback);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); // Follow url redirects.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)output);
    
    // Use this option when debugging.
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

    res = curl_easy_perform(curl);
    switch (res) { // https://curl.haxx.se/libcurl/c/libcurl-errors.html
		case CURLE_OK:
		{
			break;
		}
		case CURLE_UNSUPPORTED_PROTOCOL:
		{
			*error_message = "The URL you passed to libcurl used a protocol that this libcurl does not support. The support might be a compile-time option that you didn't use, it can be a misspelled protocol string or just a protocol libcurl has no code for.";
			break;
		}
		case CURLE_FAILED_INIT:
		{
			*error_message = "Very early initialization code failed. This is likely to be an internal error or problem, or a resource problem where something fundamental couldn't get done at init time.";
			break;
		}
		case CURLE_URL_MALFORMAT:
		{
			*error_message = "The URL was not properly formatted.";
			break;
		}
		case CURLE_NOT_BUILT_IN:
		{
			*error_message = "A requested feature, protocol or option was not found built-in in this libcurl due to a build-time decision. This means that a feature or option was not enabled or explicitly disabled when libcurl was built and in order to get it to function you have to get a rebuilt libcurl.";
			break;
		}
		case CURLE_COULDNT_RESOLVE_PROXY:
		{
			*error_message = "Couldn't resolve proxy. The given proxy host could not be resolved.";
			break;
		}
		case CURLE_COULDNT_RESOLVE_HOST:
		{
			*error_message = "Couldn't resolve host. The given remote host was not resolved.";
			break;
		}
		case CURLE_COULDNT_CONNECT:
		{
			*error_message = "Failed to connect() to host or proxy.";
			break;
		}
		/*case CURLE_WEIRD_SERVER_REPLY:
		{
			*error_message = "The server sent data libcurl couldn't parse.";
			break;
		}*/
		case CURLE_REMOTE_ACCESS_DENIED:
		{
			*error_message = "We were denied access to the resource given in the URL. For FTP, this occurs while trying to change to the remote directory.";
			break;
		}
		case CURLE_FTP_ACCEPT_FAILED:
		{
			*error_message = "While waiting for the server to connect back when an active FTP session is used, an error code was sent over the control connection or similar.";
			break;
		}
		case CURLE_FTP_WEIRD_PASS_REPLY:
		{
			*error_message = "After having sent the FTP password to the server, libcurl expects a proper reply. This error code indicates that an unexpected code was returned.";
			break;
		}
		case CURLE_FTP_ACCEPT_TIMEOUT:
		{
			*error_message = "During an active FTP session while waiting for the server to connect, the CURLOPT_ACCEPTTIMEOUT_MS (or the internal default) timeout expired.";
			break;
		}
		case CURLE_FTP_WEIRD_PASV_REPLY:
		{
			*error_message = "libcurl failed to get a sensible result back from the server as a response to either a PASV or a EPSV command. The server is flawed.";
			break;
		}
		case CURLE_FTP_WEIRD_227_FORMAT:
		{
			*error_message = "FTP servers return a 227-line as a response to a PASV command. If libcurl fails to parse that line, this return code is passed back.";
			break;
		}
		case CURLE_FTP_CANT_GET_HOST:
		{
			*error_message = "An internal failure to lookup the host used for the new connection.";
			break;
		}
		case CURLE_HTTP2:
		{
			*error_message = "A problem was detected in the HTTP2 framing layer. This is somewhat generic and can be one out of several problems, see the error buffer for details.";
			break;
		}
		case CURLE_FTP_COULDNT_SET_TYPE:
		{
			*error_message = "Received an error when trying to set the transfer mode to binary or ASCII.";
			break;
		}
		case CURLE_PARTIAL_FILE:
		{
			*error_message = "A file transfer was shorter or larger than expected. This happens when the server first reports an expected transfer size, and then delivers data that doesn't match the previously given size.";
			break;
		}
		case CURLE_FTP_COULDNT_RETR_FILE:
		{
			*error_message = "This was either a weird reply to a 'RETR' command or a zero byte transfer complete.";
			break;
		}
		case CURLE_QUOTE_ERROR:
		{
			*error_message = "When sending custom 'QUOTE' commands to the remote server, one of the commands returned an error code that was 400 or higher (for FTP) or otherwise indicated unsuccessful completion of the command.";
			break;
		}
		case CURLE_HTTP_RETURNED_ERROR:
		{
			*error_message = "This is returned if CURLOPT_FAILONERROR is set TRUE and the HTTP server returns an error code that is >= 400.";
			break;
		}
		case CURLE_WRITE_ERROR:
		{
			*error_message = "An error occurred when writing received data to a local file, or an error was returned to libcurl from a write callback.";
			break;
		}
		case CURLE_UPLOAD_FAILED:
		{
			*error_message = "Failed starting the upload. For FTP, the server typically denied the STOR command. The error buffer usually contains the server's explanation for this.";
			break;
		}
		case CURLE_READ_ERROR:
		{
			*error_message = "There was a problem reading a local file or an error returned by the read callback.";
			break;
		}
		case CURLE_OUT_OF_MEMORY:
		{
			*error_message = "A memory allocation request failed. This is serious badness and things are severely screwed up if this ever occurs.";
			break;
		}
		case CURLE_OPERATION_TIMEDOUT:
		{
			*error_message = "Operation timeout. The specified time-out period was reached according to the conditions.";
			break;
		}
		case CURLE_FTP_PORT_FAILED:
		{
			*error_message = "The FTP PORT command returned error. This mostly happens when you haven't specified a good enough address for libcurl to use. See CURLOPT_FTPPORT.";
			break;
		}
		case CURLE_FTP_COULDNT_USE_REST:
		{
			*error_message = "The FTP REST command returned error. This should never happen if the server is sane.";
			break;
		}
		case CURLE_RANGE_ERROR:
		{
			*error_message = "The server does not support or accept range requests.";
			break;
		}
		case CURLE_HTTP_POST_ERROR:
		{
			*error_message = "This is an odd error that mainly occurs due to internal confusion.";
			break;
		}
		case CURLE_SSL_CONNECT_ERROR:
		{
			*error_message = "A problem occurred somewhere in the SSL/TLS handshake. You really want the error buffer and read the message there as it pinpoints the problem slightly more. Could be certificates (file formats, paths, permissions), passwords, and others.";
			break;
		}
		case CURLE_BAD_DOWNLOAD_RESUME:
		{
			*error_message = "The download could not be resumed because the specified offset was out of the file boundary.";
			break;
		}
		case CURLE_FILE_COULDNT_READ_FILE:
		{
			*error_message = "A file given with FILE:// couldn't be opened. Most likely because the file path doesn't identify an existing file. Did you check file permissions?";
			break;
		}
		case CURLE_LDAP_CANNOT_BIND:
		{
			*error_message = "LDAP cannot bind. LDAP bind operation failed.";
			break;
		}
		case CURLE_LDAP_SEARCH_FAILED:
		{
			*error_message = "LDAP search failed.";
			break;
		}
		case CURLE_FUNCTION_NOT_FOUND:
		{
			*error_message = "Function not found. A required zlib function was not found.";
			break;
		}
		case CURLE_ABORTED_BY_CALLBACK:
		{
			*error_message = "Aborted by callback. A callback returned 'abort' to libcurl.";
			break;
		}
		case CURLE_BAD_FUNCTION_ARGUMENT:
		{
			*error_message = "A function was called with a bad parameter.";
			break;
		}
		case CURLE_INTERFACE_FAILED:
		{
			*error_message = "Interface error. A specified outgoing interface could not be used. Set which interface to use for outgoing connections' source IP address with CURLOPT_INTERFACE.";
			break;
		}
		case CURLE_TOO_MANY_REDIRECTS:
		{
			*error_message = "Too many redirects. When following redirects, libcurl hit the maximum amount. Set your limit with CURLOPT_MAXREDIRS.";
			break;
		}
		case CURLE_UNKNOWN_OPTION:
		{
			*error_message = "An option passed to libcurl is not recognized/known. Refer to the appropriate documentation. This is most likely a problem in the program that uses libcurl. The error buffer might contain more specific information about which exact option it concerns.";
			break;
		}
		case CURLE_TELNET_OPTION_SYNTAX:
		{
			*error_message = "A telnet option string was Illegally formatted.";
			break;
		}
		case CURLE_GOT_NOTHING:
		{
			*error_message = "Nothing was returned from the server, and under the circumstances, getting nothing is considered an error.";
			break;
		}
		case CURLE_SSL_ENGINE_NOTFOUND:
		{
			*error_message = "The specified crypto engine wasn't found.";
			break;
		}
		case CURLE_SSL_ENGINE_SETFAILED:
		{
			*error_message = "Failed setting the selected SSL crypto engine as default";
			break;
		}
		case CURLE_SEND_ERROR:
		{
			*error_message = "Failed sending network data.";
			break;
		}
		case CURLE_RECV_ERROR:
		{
			*error_message = "Failure with receiving network data.";
			break;
		}
		case CURLE_SSL_CERTPROBLEM:
		{
			*error_message = "problem with the local client certificate.";
			break;
		}
		case CURLE_SSL_CIPHER:
		{
			*error_message = "Couldn't use specified cipher.";
			break;
		}
		case CURLE_PEER_FAILED_VERIFICATION:
		{
			*error_message = "The remote server's SSL certificate or SSH md5 fingerprint was deemed not OK.";
			break;
		}
		case CURLE_BAD_CONTENT_ENCODING:
		{
			*error_message = "Unrecognized transfer encoding.";
			break;
		}
		case CURLE_LDAP_INVALID_URL:
		{
			*error_message = "Invalid LDAP URL.";
			break;
		}
		case CURLE_FILESIZE_EXCEEDED:
		{
			*error_message = "Maximum file size exceeded.";
			break;
		}
		case CURLE_USE_SSL_FAILED:
		{
			*error_message = "Requested FTP SSL level failed.";
			break;
		}
		case CURLE_SEND_FAIL_REWIND:
		{
			*error_message = "When doing a send operation curl had to rewind the data to retransmit, but the rewinding operation failed.";
			break;
		}
		case CURLE_SSL_ENGINE_INITFAILED:
		{
			*error_message = "Initiating the SSL Engine failed.";
			break;
		}
		case CURLE_LOGIN_DENIED:
		{
			*error_message = "The remote server denied curl to login";
			break;
		}
		case CURLE_TFTP_NOTFOUND:
		{
			*error_message = "File not found on TFTP server.";
			break;
		}
		case CURLE_TFTP_PERM:
		{
			*error_message = "Permission problem on TFTP server.";
			break;
		}
		case CURLE_REMOTE_DISK_FULL:
		{
			*error_message = "Out of disk space on the server.";
			break;
		}
		case CURLE_TFTP_ILLEGAL:
		{
			*error_message = "Illegal TFTP operation.";
			break;
		}
		case CURLE_TFTP_UNKNOWNID:
		{
			*error_message = "Unknown TFTP transfer ID.";
			break;
		}
		case CURLE_REMOTE_FILE_EXISTS:
		{
			*error_message = "File already exists and will not be overwritten.";
			break;
		}
		case CURLE_TFTP_NOSUCHUSER:
		{
			*error_message = "TFTP no such user.";
			break;
		}
		case CURLE_CONV_FAILED:
		{
			*error_message = "Character conversion failed.";
			break;
		}
		case CURLE_CONV_REQD:
		{
			*error_message = "Caller must register conversion callbacks.";
			break;
		}
		case CURLE_SSL_CACERT_BADFILE:
		{
			*error_message = "Problem with reading the SSL CA cert (path? access rights?)";
			break;
		}
		case CURLE_REMOTE_FILE_NOT_FOUND:
		{
			*error_message = "The resource referenced in the URL does not exist.";
			break;
		}
		case CURLE_SSH:
		{
			*error_message = "An unspecified error occurred during the SSH session.";
			break;
		}
		case CURLE_SSL_SHUTDOWN_FAILED:
		{
			*error_message = "Failed to shut down the SSL connection.";
			break;
		}
		case CURLE_AGAIN:
		{
			*error_message = "Socket is not ready for send/recv wait till it's ready and try again.";
			break;
		}
		case CURLE_SSL_CRL_BADFILE:
		{
			*error_message = "Failed to load CRL file.";
			break;
		}
		case CURLE_SSL_ISSUER_ERROR:
		{
			*error_message = "Issuer check failed.";
			break;
		}
		case CURLE_FTP_PRET_FAILED:
		{
			*error_message = "The FTP server does not understand the PRET command at all or does not support the given argument. Be careful when using CURLOPT_CUSTOMREQUEST, a custom LIST command will be sent with PRET CMD before PASV as well.";
			break;
		}
		case CURLE_RTSP_CSEQ_ERROR:
		{
			*error_message = "Mismatch of RTSP CSeq numbers.";
			break;
		}
		case CURLE_RTSP_SESSION_ERROR:
		{
			*error_message = "Mismatch of RTSP Session Identifiers.";
			break;
		}
		case CURLE_FTP_BAD_FILE_LIST:
		{
			*error_message = "Unable to parse FTP file list (during FTP wildcard downloading).";
			break;
		}
		case CURLE_CHUNK_FAILED:
		{
			*error_message = "Chunk callback reported error.";
			break;
		}
		case CURLE_NO_CONNECTION_AVAILABLE:
		{
			*error_message = "(For internal use only, will never be returned by libcurl) No connection available, the session will be queued.";
			break;
		}		
	}
    curl_easy_cleanup(curl);
    curl_global_cleanup();

	return output;
}

size_t readurl::write_callback(char *buf, size_t size, size_t nmemb, void *up)
{
	datastring new_item;
	stringbuilder *output = (stringbuilder *)up;
	
	new_item.length = size * nmemb;	
	if (new_item.length < 0) {
		return 0;
	}
	if (new_item.length == 0) {
		return new_item.length;
	}
	new_item.data = new char[new_item.length];
	if (new_item.data == nullptr) {
		return 0; // Memory error.
	}
	memcpy(new_item.data,buf,new_item.length);
	output->append(new_item,true);
	return new_item.length;
}
#endif
