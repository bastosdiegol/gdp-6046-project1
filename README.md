# Configuration & Deployment Project #3: MSBuild

This solution was created using Visual Studio Community 2019 Version 16 Toolset v142.

The project is currently configured to be build and run in Debug and Release mode, both on x64 architecture.

The bsae project that I'm using was the same created for Configuration & Deployment Project #2 - Tic Tac Toe Game

On the file $(SolutionDir)Project01\Project1.vcxproj from 225 you have all my custom work for this MSBuild Project

The command required to run my target can be found inside $(SolutionDir)Project01\MSBuildCommand.bat

	"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" C:\Fanshawe\gdp-6046-media\gdp-6046-project1\SoundEngine\ -t:project_03 -property:Platform=x64 -property:Configuration=Debug

For the Bonus I'm using WinRAR to compress the final folder. So please make sure my WinRAR path property on line 233 matches with the one installed on your computer

	<FileCompressorPath>"C:\Program Files\WinRAR\WinRAR.exe"</FileCompressorPath>
	
I made a short video showcasing this project:

https://www.youtube.com/watch?v=6Loupty_zLc