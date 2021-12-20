cd..
cd..
xcopy .\MapleStoryGameEngine\Bin\*.* .\MapleStoryClient2D\Bin\ /s /d /y /exclude:exclude.txt
xcopy .\MapleStoryGameEngine\Bin\*.* .\Engine\Bin\ /s /d /y /exclude:exclude.txt
xcopy .\MapleStoryGameEngine\Bin\*.* .\MapleStoryEditor\Bin\ /s /d /y /exclude:exclude.txt

xcopy .\MapleStoryGameEngine\Include\*.h .\Engine\Include\ /s /d /y
xcopy .\MapleStoryGameEngine\Include\*.inl .\Engine\Include\ /s /d /y