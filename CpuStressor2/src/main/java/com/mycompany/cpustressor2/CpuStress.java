/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 */

package com.mycompany.cpustressor2;

/**
 *
 * @author pedro
 */

public class CpuStress {
    static {
        String currentPath = System.getProperty("user.dir");        
        System.load(currentPath + "\\src\\main\\java\\com\\mycompany\\cpustressor2\\C\\cpuStress.dll");
        //System.load("C:\\Users\\pedro\\Documents\\NetBeansProjects\\CPU_STRESSOR\\CpuStressor2\\src\\main\\java\\com\\mycompany\\cpustressor2\\C\\cpuStress.dll");
        //System.load("C:\\Users\\helenas\\Documents\\GitHub\\CPU_STRESSOR\\CpuStressor2\\src\\main\\java\\com\\mycompany\\cpustressor2\\C\\cpuStress.dll");
        //System.load("C:\\Users\\helenas\\Documents\\GitHub\\CalculadoraC\\c\\libcalculadora.dll");
    }

    public native int stressCores(int[] cores, int duration, int priority);

    public native int getNumbersOfGpu();
}