# TPS

**Testing in unreal engine**

to package the project you should:

`"C:path_to_unreal_ungine\Engine\Build\BatchFiles\RunUAT.bat" ^ BuildCookRun ^ -project="path_to_project.uproject" ^ -platform=Win64 -clientconfig=Shipping ^ -archivedirectory="path to build" ^ -build -cook -stage -stagingdirectory="create_a_stage_folder\Stage" ^ -archive -pak -package`

paste it to build.bat file into your project directory and then run it
