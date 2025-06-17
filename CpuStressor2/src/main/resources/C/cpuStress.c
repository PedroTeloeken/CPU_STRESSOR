#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <jni.h>
#include "com_mycompany_cpustressor2_CpuStress.h"

typedef struct {
    int duration;
    int core;
    int priority; // De 1 (MIN) a 10 (MAX)
} ThreadParams;


DWORD mapJavaPriorityToProcessClass(int javaPriority) {
    if (javaPriority <= 2) return IDLE_PRIORITY_CLASS;
    else if (javaPriority == 5) return NORMAL_PRIORITY_CLASS;
    else if (javaPriority <= 10) return HIGH_PRIORITY_CLASS;
    else return REALTIME_PRIORITY_CLASS;
}

DWORD WINAPI stress_cpu(LPVOID lpParam) {
    ThreadParams* params = (ThreadParams*)lpParam;
    HANDLE thread = GetCurrentThread();
    DWORD_PTR mask = 1ULL << params->core;

    SetThreadAffinityMask(thread, mask);

    volatile double x = 0.0001;
    time_t start = time(NULL);

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

JNIEXPORT jint JNICALL Java_com_mycompany_cpustressor2_CpuStress_stressCores(JNIEnv* env, jobject obj, jintArray cores, jint duration, jint priority) {
    // Define prioridade do processo
    HANDLE processoAtual = GetCurrentProcess();
    DWORD classePrioridade = mapJavaPriorityToProcessClass(priority);
    if (!SetPriorityClass(processoAtual, classePrioridade)) {
        printf("Erro ao definir prioridade do processo: %lu\n", GetLastError());
    } else {
        printf("Prioridade do processo definida para %lu\n", classePrioridade);
    }

    // Código das threads
    jsize length = (*env)->GetArrayLength(env, cores);
    jint* coreArray = (*env)->GetIntArrayElements(env, cores, NULL);
    HANDLE* threads = malloc(length * sizeof(HANDLE));

    for (jsize i = 0; i < length; i++) {
        ThreadParams* params = malloc(sizeof(ThreadParams));
        params->duration = duration;
        params->core = coreArray[i];

        threads[i] = CreateThread(NULL, 0, stress_cpu, params, 0, NULL);
        if (threads[i] == NULL) {
            printf("Erro ao criar a thread %d\n", i);
            free(params);
        }
    }

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
