#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <jni.h>
#include "com_mycompany_cpustressor2_CpuStress.h"

//struct chamada ThreadParamns, que é um agrupamento de variáveis
typedef struct {
    int duration;
    int core;
} ThreadParams;

//Declara a função que será executada pela thread do Windows (WINAPI com LPVOID como parâmetro genérico).
DWORD WINAPI stress_cpu(LPVOID lpParam) {
 //Converte o ponteiro genérico void* para o tipo ThreadParams*.
    ThreadParams* params = (ThreadParams*)lpParam;
    //Obtém o identificador da thread atual.
    HANDLE thread = GetCurrentThread();
    //Cria uma máscara de afinidade, ativando apenas o bit correspondente ao núcleo escolhido. Ex: se core = 2, então mask = 0b00000100 (ou 4), ativando só o núcleo lógico 2.
    DWORD_PTR mask = 1ULL << params->core;
    //Associa essa thread apenas ao núcleo especificado.
    SetThreadAffinityMask(thread, mask);  // Afina o thread ao core especificado

    //Variável usada nos cálculos intensivos. O volatile impede otimizações excessivas do compilador.
    volatile double x = 0.0001;
    //Captura o tempo inicial (timestamp atual).
    time_t start = time(NULL);

    //Loop principal que roda por duration segundos
    while (difftime(time(NULL), start) < params->duration) {
        for (int i = 0; i < 1000000; i++) {
            x = x * 1.0000001 + sin(x) * cos(x);
            x = sqrt(x + 1.2345);
            x = log(x + 1.0001) * exp(x);
            x = x / (sin(x) + 1.00001);
        }
    }

    printf("Thread do núcleo %d finalizada. Valor final: %f\n", params->core, x);
    free(params);
    return 0;
}

// JNI para chamar a função via Java
JNIEXPORT jint JNICALL Java_com_mycompany_cpustressor2_CpuStress_stressCores(JNIEnv* env, jobject obj, jintArray cores, jint duration) {
    // Obtém o tamanho do array de núcleos
    jsize length = (*env)->GetArrayLength(env, cores);
    jint* coreArray = (*env)->GetIntArrayElements(env, cores, NULL);

    HANDLE* threads = malloc(length * sizeof(HANDLE));

    for (jsize i = 0; i < length; i++) {
        ThreadParams* params = malloc(sizeof(ThreadParams));
        params->duration = duration;
        params->core = coreArray[i];

        threads[i] = CreateThread(NULL, 0, stress_cpu, params, 0, NULL);
    }

    // Espera todas as threads terminarem
    WaitForMultipleObjects(length, threads, TRUE, INFINITE);

    for (jsize i = 0; i < length; i++) {
        CloseHandle(threads[i]);
    }

    free(threads);
    (*env)->ReleaseIntArrayElements(env, cores, coreArray, 0);

    return 0;
}

JNIEXPORT jint JNICALL Java_com_mycompany_cpustressor2_CpuStress_getNumbersOfGpu(JNIEnv* env, jobject obj) {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return (jint) sysinfo.dwNumberOfProcessors;
}