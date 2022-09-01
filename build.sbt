enablePlugins(ScalaNativePlugin)
scalaVersion := "3.1.3"
nativeLinkStubs := true
libraryDependencies ++= Seq(
  "com.github.lolgab" %%% "native-loop-core" % "0.2.1",
)
