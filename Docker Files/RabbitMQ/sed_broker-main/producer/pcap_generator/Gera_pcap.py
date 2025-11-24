"""
======================================================================
Universidade Federal de Juiz de Fora
Autor: Adaptado do código original de Victor Mendes Ribeiro

Descrição:
Este código gera um arquivo PCAP artificial com pacotes no formato IEC 61850 (SV).
Os dados são montados e organizados de acordo com o protocolo IEC 61850.
======================================================================
"""

import struct
import numpy as np
from scapy.all import *
import scapy.utils

def cria_subpacote(valores):
    """Cria um subpacote a partir de uma lista de valores e formatos"""
    pacote = b''
    for valor, formato in valores:
        pacote += struct.pack(formato, valor)
    return pacote

def cria_cabeçalhoFIXO():
    """Cria o cabeçalho fixo do pacote IEC 61850 SV"""
    # Ethernet - Perfil T
    Mac_destino = "02:0C:CD:04:00:00"
    Mac_origem = "00:50:C2:4F:91:99"
    ethertype = 0x88ba

    # SV - Dispositivo fisico
    SV_APPID = [0x4001, "!H"]
    SV_Length = [0x036b, "!H"]
    SV_reserved1 = [0x0000, "!H"]
    SV_reserved2 = [0x0000, "!H"]

    # savPDU - Dispositivo Logico
    savPDU = [0x60, "!B"]
    savPDU_Length = [0x82, "!B"]
    savPDU_Values = [0x035f, "!H"]

    # noASDU - No Logico
    noASDU_tag = [0x80, "!B"]
    noASDU_Length = [0x01, "!B"]
    noASDU_Values = [0x08, "!B"]
    noASDU_Seq_ASDU = [0xA2, "!B"]
    noASDU_Seq_Length = [0x82, "!B"]
    noPDU_Values = [0x0358, "!H"]

    # Cria o pacote Ethernet
    ethernet_frame = Ether(dst=Mac_destino, src=Mac_origem, type=ethertype)

    # Cria os subpacotes
    packet_SV = cria_subpacote([SV_APPID, SV_Length, SV_reserved1, SV_reserved2])
    packet_savPDU = cria_subpacote([savPDU, savPDU_Length, savPDU_Values])
    packet_noASDU = cria_subpacote([noASDU_tag, noASDU_Length, noASDU_Values,
                                    noASDU_Seq_ASDU, noASDU_Seq_Length, noPDU_Values])

    return ethernet_frame / Raw(load=packet_SV + packet_savPDU + packet_noASDU)

def cria_pacote_ASDU(smpCnt, IA, IAq, IB, IBq, IC, ICq, IN, INq, VA, VAq, VB, VBq, VC, VCq, VN, VNq):
    """Cria um pacote ASDU com dados de amostras"""
    # ASDU1 - Data Objeto
    ASDU = [0x30, "!B"]
    ASDU_Lenth = [0x69, "!B"]

    # svID
    ASDU_svID = [0x80, "!B"]
    ASDU_svID_Lenth = [0x0A, "!B"]
    ASDU_svID_Values_1 = [0x3030, "!H"]
    ASDU_svID_Values_2 = [0x30304D5530313032, "!Q"]

    # smpCnt - Contagem de amostras
    ASDU_smpCnt = [0x82, "!B"]
    ASDU_smpCnt_Lenth = [0x02, "!B"]
    ASDU_smpCnt_Values = [int(smpCnt), "!H"]

    # confRev - Revisão de Configuração
    ASDU_confRev = [0x83, "!B"]
    ASDU_confRev_Lenth = [0x04, "!B"]
    ASDU_confRev_Values = [0x00086880, "!I"]

    # refrTm - Hora de Atualização
    ASDU_refrTm = [0x84, "!B"]
    ASDU_refrTm_Lenth = [0x08, "!B"]
    ASDU_refrTm_Values_1 = [0x65492744, "!I"]
    ASDU_refrTm_Values_2 = [0x00000200, "!I"]

    # smpSynch
    ASDU_smpSynch = [0x85, "!B"]
    ASDU_smpSynch_Lenth = [0x01, "!B"]
    ASDU_smpSynch_Values = [0x01, "!B"]

    # smpRate
    ASDU_smpRate = [0x86, "!B"]
    ASDU_smpRate_Lenth = [0x02, "!B"]
    ASDU_smpRate_Values = [0x8688, "!H"]

    # SeqSample - Amostras
    ASDU_SeqSample = [0x87, "!B"]
    ASDU_SeqSample_Lenth = [0x40, "!B"]

    ASDU_SeqSample_IA = [IA, "!i"]
    ASDU_SeqSample_IAq = [IAq, "!I"]
    ASDU_SeqSample_IB = [IB, "!i"]
    ASDU_SeqSample_IBq = [IBq, "!I"]
    ASDU_SeqSample_IC = [IC, "!i"]
    ASDU_SeqSample_ICq = [ICq, "!I"]
    ASDU_SeqSample_IN = [IN, "!i"]
    ASDU_SeqSample_INq = [INq, "!I"]

    ASDU_SeqSample_VA = [VA, "!i"]
    ASDU_SeqSample_VAq = [VAq, "!I"]
    ASDU_SeqSample_VB = [VB, "!i"]
    ASDU_SeqSample_VBq = [VBq, "!I"]
    ASDU_SeqSample_VC = [VC, "!i"]
    ASDU_SeqSample_VCq = [VCq, "!I"]
    ASDU_SeqSample_VN = [VN, "!i"]
    ASDU_SeqSample_VNq = [VNq, "!I"]

    # Constrói o pacote ASDU completo
    packet_ASDU = cria_subpacote([ASDU, ASDU_Lenth])
    packet_svID = cria_subpacote([ASDU_svID, ASDU_svID_Lenth, ASDU_svID_Values_1, ASDU_svID_Values_2])
    packet_smpCnt = cria_subpacote([ASDU_smpCnt, ASDU_smpCnt_Lenth, ASDU_smpCnt_Values])
    packet_confRev = cria_subpacote([ASDU_confRev, ASDU_confRev_Lenth, ASDU_confRev_Values])
    packet_refrTm = cria_subpacote([ASDU_refrTm, ASDU_refrTm_Lenth, ASDU_refrTm_Values_1, ASDU_refrTm_Values_2])
    packet_smpSynch = cria_subpacote([ASDU_smpSynch, ASDU_smpSynch_Lenth, ASDU_smpSynch_Values])
    packet_smpRate = cria_subpacote([ASDU_smpRate, ASDU_smpRate_Lenth, ASDU_smpRate_Values])
    packet_SeqSample = cria_subpacote(
        [ASDU_SeqSample, ASDU_SeqSample_Lenth,
         ASDU_SeqSample_IA, ASDU_SeqSample_IAq,
         ASDU_SeqSample_IB, ASDU_SeqSample_IBq,
         ASDU_SeqSample_IC, ASDU_SeqSample_ICq,
         ASDU_SeqSample_IN, ASDU_SeqSample_INq,
         ASDU_SeqSample_VA, ASDU_SeqSample_VAq,
         ASDU_SeqSample_VB, ASDU_SeqSample_VBq,
         ASDU_SeqSample_VC, ASDU_SeqSample_VCq,
         ASDU_SeqSample_VN, ASDU_SeqSample_VNq])

    return Raw(load=packet_ASDU + packet_svID + packet_smpCnt + packet_confRev +
               packet_refrTm + packet_smpSynch + packet_smpRate + packet_SeqSample)

def gerar_sinais_trifasicos():
    """Gera sinais trifásicos simulados"""
    frequencia = 60
    taxa_aquisicao = 15360
    duracao_simulacao = 1
    num_amostras = int(taxa_aquisicao * duracao_simulacao)

    t = np.arange(0, duracao_simulacao, 1 / taxa_aquisicao)

    amplitude = 2**16
    offset = 0
    fase_a = 0
    fase_b = 2 * np.pi / 3
    fase_c = 4 * np.pi / 3

    # Gera tensões
    # V_A = np.zeros(len(t))
    # V_B = -1*(2**30) *np.ones(len(t))
    # V_C = (2**30) *np.ones(len(t))
    # V_N = (2**16) *np.ones(len(t))

    # I_A = np.zeros(len(t))
    # I_B = (2**30) *np.ones(len(t))
    # I_C = (2**30) *np.ones(len(t))
    # I_N = (2**16) *np.ones(len(t))

    # Gera tensões
    V_A = offset + amplitude * np.sin(2 * np.pi * frequencia * t + fase_a)
    V_B = offset + amplitude * np.sin(2 * np.pi * frequencia * t + fase_b) 
    V_C = offset + amplitude * np.sin(2 * np.pi * frequencia * t + fase_c)
    V_N = (V_A + V_B + V_C) / 3

    # Gera correntes
    I_A = offset + amplitude/2 * np.sin(2 * np.pi * frequencia * t + fase_a)
    I_B = offset + amplitude/2 * np.sin(2 * np.pi * frequencia * t + fase_b)
    I_C = offset + amplitude/2 * np.sin(2 * np.pi * frequencia * t + fase_c)
    I_N = (I_A + I_B + I_C) / 3

    import plotly.graph_objects as go

    fig = go.Figure()

    # Adicionar as linhas para cada fase
    fig.add_trace(go.Scatter(x=t, y=V_A, mode='lines', name='Tensão Fase A', line=dict(width=2)))
    fig.add_trace(go.Scatter(x=t, y=V_B, mode='lines', name='Tensão Fase B', line=dict(width=2)))
    fig.add_trace(go.Scatter(x=t, y=V_C, mode='lines', name='Tensão Fase C', line=dict(width=2)))
    fig.add_trace(go.Scatter(x=t, y=V_N, mode='lines', name='Tensão Neutro', line=dict(width=2)))
    fig.add_trace(go.Scatter(x=t, y=I_A, mode='lines', name='Corrente Fase A', line=dict(width=2)))
    fig.add_trace(go.Scatter(x=t, y=I_B, mode='lines', name='Corrente Fase B', line=dict(width=2)))
    fig.add_trace(go.Scatter(x=t, y=I_C, mode='lines', name='Corrente Fase C', line=dict(width=2)))
    fig.add_trace(go.Scatter(x=t, y=I_N, mode='lines', name='Corrente Neutro', line=dict(width=2)))

    # Configurar o layout
    fig.update_layout(
        title='Simulação Trifásica 60Hz com Neutro',
        xaxis_title='Tempo (s)',
        yaxis_title='Amplitude',
        # width=1000,  # equivalente ao figsize=(10, 6) - 10*100 = 1000
        # height=600,  # 6*100 = 600
        showlegend=True,
        template='plotly_dark'  # ou outro template de sua preferência
    )

    # Adicionar grid
    # fig.update_xaxis(showgrid=True, gridwidth=1, gridcolor='lightgray')
    # fig.update_yaxis(showgrid=True, gridwidth=1, gridcolor='lightgray')

    fig.show()

    return V_A, V_B, V_C, V_N, I_A, I_B, I_C, I_N, t

def gerar_pcap_artificial(nome_arquivo="iec61850_sv.pcap", num_pacotes=10):
    """
    Gera um arquivo PCAP artificial com pacotes IEC 61850 SV

    Args:
        nome_arquivo (str): Nome do arquivo PCAP de saída
        num_pacotes (int): Número de pacotes a serem gerados
    """

    print(f"Gerando {num_pacotes} pacotes IEC 61850 SV...")

    # Gera os sinais trifásicos
    V_A, V_B, V_C, V_N, I_A, I_B, I_C, I_N, t = gerar_sinais_trifasicos()

    pacotes = []
    packet_Fixo = cria_cabeçalhoFIXO()

    i = 0
    total_amostras = len(t)

    for pacote_num in range(num_pacotes):
        # Cria múltiplos ASDUs por pacote (8 neste exemplo)
        packet_ASDUs = Raw()
        for n in range(8):
            if i >= total_amostras:
                i = 0  # Reinicia se atingir o final dos dados

            # Cria um ASDU com os dados atuais
            asdu = cria_pacote_ASDU(
                i, int(I_A[i]), 1012,
                int(I_B[i]), 1012,
                int(I_C[i]), 1012,
                int(I_N[i]), 1012,
                int(V_A[i]), 1012,
                int(V_B[i]), 1012,
                int(V_C[i]), 1012,
                int(V_N[i]), 1012
            )

            packet_ASDUs = packet_ASDUs / asdu
            i += 1

        # Combina cabeçalho fixo com ASDUs
        pacote_completo = packet_Fixo / packet_ASDUs

        # Adiciona timestamp ao pacote
        pacote_completo.time = pacote_num * 0.001  # Timestamp incremental

        pacotes.append(pacote_completo)

        print(f"Pacote {pacote_num + 1}/{num_pacotes} gerado - Amostra {i}")

    # Salva no arquivo PCAP
    wrpcap(nome_arquivo, pacotes)
    print(f"Arquivo PCAP salvo como: {nome_arquivo}")
    print(f"Total de pacotes gerados: {len(pacotes)}")

# Exemplo de uso
Fs = 15360
Tempo = 1 #seg
Tempo = int(Tempo) #seg
numero_de_pacotes = int(1 // (8/Fs))
print('Numero de Pacotes: ',numero_de_pacotes)

# Gera um PCAP com 'numero_de_pacotes' pacotes IEC 61850 SV
gerar_pcap_artificial("iec61850_Teste1.pcap", numero_de_pacotes)
print("PCAP gerado com sucesso!")