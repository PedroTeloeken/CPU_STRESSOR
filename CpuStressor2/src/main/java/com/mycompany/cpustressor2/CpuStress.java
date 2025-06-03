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
        //System.load("C:\\Users\\pedro\\Documents\\NetBeansProjects\\CpuStressor2\\src\\main\\java\\com\\mycompany\\cpustressor2\\C\\cpuStress.dll");
        System.load("C:\\Users\\helenas\\Documents\\GitHub\\CPU_STRESSOR\\CpuStressor2\\src\\main\\java\\com\\mycompany\\cpustressor2\\C\\cpuStress.dll");
        //System.load("C:\\Users\\helenas\\Documents\\GitHub\\CalculadoraC\\c\\libcalculadora.dll");
    }

    public native int stressCores(int[] cores, int duration);

    public native int getNumbersOfGpu();

    public static void main(String[] args) {

        CpuStress cpu = new CpuStress();

        System.out.println("Número de núcleos desse PC é de " + cpu.getNumbersOfGpu());
        
        int[] cores = {0 , 1};
        cpu.stressCores(cores, 20);

        System.out.println("Terminou");
    }
}
