#pragma once
#include <manager/exceptions.h>
#include <storage/exceptions.h>
#include <utils/exceptions.h>
#include <vk-api/exceptions.h>

namespace util{

class ExceptionsHandler {
public:
    virtual ~ExceptionsHandler() {}
    virtual void Handle(const util::FileWriteException& e) = 0;
    virtual void Handle(const util::FileReadException& e) = 0;
    virtual void Handle(const util::FileException& e) = 0;
    virtual void Handle(const util::json::TypeUInt64Exception& e) = 0;
    virtual void Handle(const util::json::TypeInt64Exception& e) = 0;
    virtual void Handle(const util::json::TypeStringException& e) = 0;
    virtual void Handle(const util::json::NotAnArrayException& e) = 0;
    virtual void Handle(const util::json::NotAnObjectException& e) = 0;
    virtual void Handle(const util::json::TypeException& e) = 0;
    virtual void Handle(const util::json::NoFieldException& e) = 0;
    virtual void Handle(const util::json::Exception& e) = 0;
    virtual void Handle(const storage::PathCreateException& e) = 0;
    virtual void Handle(const storage::PathIsFileException& e) = 0;
    virtual void Handle(const storage::PathException& e) = 0;
    virtual void Handle(const vk_api::RequestParseException& e) = 0;
    virtual void Handle(const vk_api::RequestException& e) = 0;
    virtual void Handle(const vk_api::ApiException& e) = 0;
    virtual void Handle(const manager::NoApplicationIdException& e) = 0;
    virtual void Handle(const manager::NoTokenException& e) = 0;
    virtual void Handle(const util::BasicException& e) = 0;
    virtual void Handle(const std::runtime_error& e) = 0;
    virtual void Handle(const std::exception& e) = 0;
    virtual void Handle() = 0; // Handle unknown exception

    template<class Func, class ...Args>
    void ProcessFunction(Func&& func, Args&& ...args) {
        try {
            func(std::forward<Args>(args)...);
        } catch (const util::FileWriteException& e) {
            Handle(e);
        } catch (const util::FileReadException& e) {
            Handle(e);
        } catch (const util::FileException& e) {
            Handle(e);
        } catch (const util::json::TypeUInt64Exception& e) {
            Handle(e);
        } catch (const util::json::TypeInt64Exception& e) {
            Handle(e);
        } catch (const util::json::TypeStringException& e) {
            Handle(e);
        } catch (const util::json::NotAnArrayException& e) {
            Handle(e);
        } catch (const util::json::NotAnObjectException& e) {
            Handle(e);
        } catch (const util::json::NoFieldException& e) {
            Handle(e);
        } catch (const util::json::TypeException& e) {
            Handle(e);
        } catch (const util::json::Exception& e) {
            Handle(e);
        } catch (const storage::PathCreateException& e) {
            Handle(e);
        } catch (const storage::PathIsFileException& e) {
            Handle(e);
        } catch (const storage::PathException& e) {
            Handle(e);
        } catch (const vk_api::RequestParseException& e) {
            Handle(e);
        } catch (const vk_api::RequestException& e) {
            Handle(e);
        } catch (const vk_api::ApiException& e) {
            Handle(e);
        } catch (const manager::NoApplicationIdException& e) {
            Handle(e);
        } catch (const manager::NoTokenException& e) {
            Handle(e);
        } catch (const util::BasicException& e) {
            Handle(e);
        } catch (const std::runtime_error& e) {
            Handle(e);
        } catch (const std::exception& e) {
            Handle(e);
        } catch (...) {
            Handle();
        }
    }
};

}
