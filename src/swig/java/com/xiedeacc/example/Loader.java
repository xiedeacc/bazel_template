package com.xiedeacc.example;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class Loader {
  public static boolean loaded = false;

  public static synchronized void loadNativeLibraries() {
    String soName = "jni_config_manager";

    if (!loaded) {
      try {
        System.loadLibrary(soName);
        loaded = true;
      } catch (UnsatisfiedLinkError e) {
        System.out.println("load " + soName + "error");
        e.printStackTrace();
      }
      System.out.println("load " + soName + "success");
    }
  }
}
