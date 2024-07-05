#include "NativeScriptsRegister.h"

#include "PlayerController.h"
#include "NativeScriptsLoader.h"

NativeScriptRegisterer::NativeScriptRegisterer()
{
	NativeScriptsLoader::instance->registerScript("PlayerController", []() { return new PlayerController(); });
}
