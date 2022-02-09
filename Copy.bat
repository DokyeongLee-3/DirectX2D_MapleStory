cd..
cd..
xcopy .\MapleStoryGameEngine\Bin\*.* .\MapleStoryClient2D\Bin\ /s /d /y /exclude:exclude.txt
xcopy .\MapleStoryGameEngine\Bin\*.* .\Engine\Bin\ /s /d /y /exclude:exclude.txt
xcopy .\MapleStoryGameEngine\Bin\*.* .\MapleStoryEditor\Bin\ /s /d /y /exclude:exclude.txt

xcopy .\MapleStoryGameEngine\Include\*.h .\Engine\Include\ /s /d /y
xcopy .\MapleStoryGameEngine\Include\*.inl .\Engine\Include\ /s /d /y
xcopy .\MapleStoryGameEngine\Include\*.hpp .\Engine\Include\ /s /d /y

rem xcopy .\MapleStoryEditor\Include\Object\Stage.h				.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\Stage.cpp			.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\StaticMapObj.h		.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\StaticMapObj.cpp	.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\Portal.h		.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\Portal.cpp	.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\DoubleHelixBlinkTree.h		.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\DoubleHelixBlinkTree.cpp	.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\Butterfly.h		.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\Butterfly.cpp	.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\LampLight.h		.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\LampLight.cpp	.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\BlinkTree.h		.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\BlinkTree.cpp	.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\LobbyBigLamp.h		.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\LobbyBigLamp.cpp	.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\LobbySmallLamp.h		.\MapleStoryClient2D\Include\Object\ /s /d /y
rem xcopy .\MapleStoryEditor\Include\Object\LobbySmallLamp.cpp	.\MapleStoryClient2D\Include\Object\ /s /d /y

xcopy .\MapleStoryEditor\Bin\Texture\*.*	.\MapleStoryClient2D\Bin\Texture\ /s /d /y
xcopy .\MapleStoryEditor\Bin\Animation\*.*	.\MapleStoryClient2D\Bin\Animation\ /s /d /y
xcopy .\MapleStoryEditor\Bin\Scene\*.*		.\MapleStoryClient2D\Bin\Scene\ /s /d /y

rem xcopy .\MapleStoryEditor\Include\Component\DragCollider.*	.\MapleStoryClient2D\Include\Component\ /s /d /y