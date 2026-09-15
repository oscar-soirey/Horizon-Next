#pragma once

#include <sstream>



#include <thread>
#include <atomic>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#define PIPE_FUNC _pipe
#define DUP_FUNC _dup
#define DUP2_FUNC _dup2
#define CLOSE_FUNC _close
#define READ_FUNC _read
#else
#include <unistd.h>
#define DUP_FUNC dup
#define DUP2_FUNC dup2
#define CLOSE_FUNC close
#define READ_FUNC read
#endif


#include <OgreLogManager.h>


namespace hn
{
	class Engine;
}

//std::cout
class EngineStreambuf : public std::streambuf {
public:
	EngineStreambuf(hn::Engine* engine): engine_(engine){}
protected:
	int overflow(int c) override;
	int sync() override;

private:
	void flushLine();

	hn::Engine* engine_=nullptr;
	std::string buffer_;
};



//printf
class PrintfInterceptor {
public:
	explicit PrintfInterceptor(hn::Engine* engine);
	~PrintfInterceptor();

private:
	void ReadLoop();

	hn::Engine* engine_;
	int pipe_fd_[2] = {-1, -1};
	int saved_stdout_ = -1;
	std::thread reader_thread_;
	std::atomic<bool> running_{false};
};




//Ogre outputs
class OgreLogRedirector : public Ogre::LogListener {
public:
	explicit OgreLogRedirector(hn::Engine* engine);

	void messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml,
											bool maskDebug, const Ogre::String& logName,
											bool& skipThisMessage) override;

private:
	hn::Engine* engine_;
};