/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 */

package com.mycompany.cpustressor2;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 *
 * @author pedro teloeken
 */

public class CpuStress {
     static {
        try {
            // Lê a DLL do classpath
            InputStream in = CpuStress.class.getResourceAsStream("/C/CpuStress.dll");
            if (in == null) {
                System.out.print("A DLL não foi encontrada no classpath");
                throw new RuntimeException("DLL não encontrada no classpath.");
            }

            // Cria um arquivo temporário
            File tempDll = File.createTempFile("cpuStress", ".dll");
            tempDll.deleteOnExit();

            // Copia o conteúdo da DLL para o arquivo temporário
            try (OutputStream out = new FileOutputStream(tempDll)) {
                byte[] buffer = new byte[1024];
                int bytesRead;
                while ((bytesRead = in.read(buffer)) != -1) {
                    out.write(buffer, 0, bytesRead);
                }
            }

            // Carrega a DLL
            System.load(tempDll.getAbsolutePath());

        } catch (IOException | RuntimeException e) {
            e.printStackTrace();
        }
    }

    public native int stressCores(int[] cores, int duration, int priority);

    public native int getNumbersOfGpu();
}