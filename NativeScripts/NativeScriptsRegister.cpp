#include "NativeScriptsRegister.h"

#include "PlayerController.h"
#include "CameraScript.h"
#include "EnemyController.h"
#include "CursorScript.h"

#include "NativeScriptsLoader.h"

NativeScriptRegisterer::NativeScriptRegisterer()
{
	NativeScriptsLoader::instance->registerScript("PlayerController", []() { return new PlayerController(); });
	NativeScriptsLoader::instance->registerScript("CameraScript", []() { return new CameraScript(); });
	NativeScriptsLoader::instance->registerScript("EnemyController", []() { return new EnemyController(); });
	NativeScriptsLoader::instance->registerScript("CursorScript", []() { return new CursorScript(); });
}
