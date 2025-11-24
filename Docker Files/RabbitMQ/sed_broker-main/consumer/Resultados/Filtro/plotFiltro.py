import numpy as np
import plotly.graph_objects as go
from plotly.subplots import make_subplots

# Frequência de amostragem (mesma usada na classe C++)
FS = 15360  

# Arquivos esperados (na mesma pasta do script)
ARQ_ENTRADA = "SinalEntrada.txt"
ARQ_FILTRADO = "SinalFiltrado.txt"

# ------------------------------
# Leitura dos arquivos
# ------------------------------
entrada = np.loadtxt(ARQ_ENTRADA)
filtrado = np.loadtxt(ARQ_FILTRADO)

# Garante que os dois sinais tenham o mesmo tamanho para plot
N = min(len(entrada), len(filtrado))
entrada = entrada[:N]
filtrado = filtrado[:N]

# Vetor de tempo
tempo = np.arange(N) / FS

# ------------------------------
# Criar figura com 2 subplots
# ------------------------------
fig = make_subplots(
    rows=2,
    cols=1,
    subplot_titles=["Sinal Bruto (Entrada)", "Sinal Filtrado"],
    shared_xaxes=True,
    vertical_spacing=0.10
)

# ---- Plot 1: Entrada ----
fig.add_trace(
    go.Scatter(
        x=tempo,
        y=entrada,
        mode="lines",
        name="Entrada",
        line=dict(width=1)
    ),
    row=1, col=1
)

# ---- Plot 2: Filtrado ----
fig.add_trace(
    go.Scatter(
        x=tempo,
        y=filtrado,
        mode="lines",
        name="Filtrado",
        line=dict(width=1)
    ),
    row=2, col=1
)

# ------------------------------
# Layout
# ------------------------------
fig.update_layout(
    height=900,
    title_text="Sinais de Entrada e Sinal Filtrado",
    template="plotly_dark",
    showlegend=False
)

fig.update_yaxes(title_text="Amplitude", row=1, col=1)
fig.update_yaxes(title_text="Amplitude Filtrada", row=2, col=1)
fig.update_xaxes(title_text="Tempo (s)", row=2, col=1)

# ------------------------------
# Mostrar no navegador
# ------------------------------
fig.show(renderer="browser")
