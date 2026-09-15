#include "redirect.h"

#include "../engine.h"
#include <iostream>


int EngineStreambuf::overflow(int c)
{
	if (c != EOF)
	{
		buffer_ += static_cast<char>(c);
		if (c == '\n')
		{
			flushLine();
		}
	}
	return c;
}

int EngineStreambuf::sync()
{
	if (!buffer_.empty()) flushLine();
	return 0;
}

void EngineStreambuf::flushLine()
{
	//what to do with the captured text
	if (engine_->logger_)
	{
		engine_->logger_->RedirectMessages(buffer_);
	}
	buffer_.clear();
}




PrintfInterceptor::PrintfInterceptor(hn::Engine* engine) : engine_(engine)
{
#ifdef _WIN32
	PIPE_FUNC(pipe_fd_, 4096, O_TEXT);
#else
	pipe(pipe_fd_);
#endif
	saved_stdout_ = DUP_FUNC(fileno(stdout));

	DUP2_FUNC(pipe_fd_[1], fileno(stdout));
	CLOSE_FUNC(pipe_fd_[1]); // stdout garde une copie du fd via dup2, on peut fermer l'original

	setvbuf(stdout, nullptr, _IONBF, 0); // désactive le buffering, sinon les lignes arrivent en retard

	running_ = true;
	reader_thread_ = std::thread(&PrintfInterceptor::ReadLoop, this);
}

PrintfInterceptor::~PrintfInterceptor()
{
	fflush(stdout);

	// restaure stdout AVANT de fermer le pipe, pour que printf() redevienne normal
	DUP2_FUNC(saved_stdout_, fileno(stdout));
	CLOSE_FUNC(saved_stdout_);

	running_ = false;
	CLOSE_FUNC(pipe_fd_[0]); // débloque le read() en cours dans le thread
	if (reader_thread_.joinable()) reader_thread_.join();
}

void PrintfInterceptor::ReadLoop()
{
	char buf[1024];
	std::string line_buffer;

	while (running_)
	{
		int n = READ_FUNC(pipe_fd_[0], buf, sizeof(buf) - 1);
		if (n <= 0) break; // pipe fermé ou erreur -> on sort

		buf[n] = '\0';
		line_buffer += buf;

		size_t pos;
		while ((pos = line_buffer.find('\n')) != std::string::npos)
		{
			std::string line = line_buffer.substr(0, pos);
			line_buffer.erase(0, pos + 1);

			if (engine_->logger_)
				engine_->logger_->RedirectMessages(line);
		}
	}
}



OgreLogRedirector::OgreLogRedirector(hn::Engine* engine) : engine_(engine) {}

void OgreLogRedirector::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName, bool& skipThisMessage)
{
	if (!engine_->logger_) return;

	hn::ELog_type type = hn::MSG;
	if (lml == Ogre::LML_CRITICAL) type = hn::ERR;

	engine_->logger_->RedirectMessages(message);

	//skipThisMessage = false; //pour empecher l'ecriture sur ogre.log
}