package bazel_template.swig;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

enum Os {
  UNKNOW,
  UBUNTU_14_04,
  UBUNTU_16_04,
  UBUNTU_18_04,
  UBUNTU_20_04,
  UBUNTU_22_04,
}


public class Loader {
  public static boolean loaded = false;

  public static Os getOsVersion() {
    String fileName = "/etc/issue";
    if (!Files.exists(Paths.get(fileName))) {
      return Os.UNKNOW;
    }

    String content = "";
    try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
      String line;
      while ((line = br.readLine()) != null) {
        content += line;
      }
    } catch (IOException e) {
      e.printStackTrace();
      return Os.UBUNTU_22_04;
    }

    if (content.contains("Ubuntu") && content.contains("22")) {
      return Os.UBUNTU_22_04;
    } else if (content.contains("Ubuntu") && content.contains("20")) {
      return Os.UBUNTU_20_04;
    } else if (content.contains("Ubuntu") && content.contains("18")) {
      return Os.UBUNTU_18_04;
    } else if (content.contains("Ubuntu") && content.contains("16")) {
      return Os.UBUNTU_16_04;
    } else if (content.contains("Ubuntu") && content.contains("14")) {
      return Os.UBUNTU_14_04;
    }
    return Os.UBUNTU_22_04;
  }


  public static synchronized void loadNativeLibraries() {
    String soName = new String();
    Os os = getOsVersion();
    if (os == Os.UBUNTU_22_04) {
      soName = "jni_bazel_template_config.22.04";
    } else if (os == Os.UBUNTU_20_04) {
      soName = "jni_bazel_template_config.20.04";
    } else if (os == Os.UBUNTU_18_04) {
      soName = "jni_bazel_template_config.18.04";
    } else if (os == Os.UBUNTU_16_04) {
      soName = "jni_bazel_template_config.16.04";
    } else if (os == Os.UBUNTU_14_04) {
      soName = "jni_bazel_template_config.14.04";
    } else {
      soName = "jni_bazel_template_config.22.04";
    }

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
