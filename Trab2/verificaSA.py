class SA:
    def __init__(self, quantsensores):
        self.position = 0
        self.valores = [[0]*3]*60                  # guarda [temp,id_sensor,id_leitura]
        self.idLeitura = [0]*quantsensores           # guarda a quantidade de leituras para cada sensor

    def sensor(self, id, temperatura):
        self.valores[self.position] = [temperatura, id, self.idLeitura[id]]
        self.idLeitura[id] = self.idLeitura[id] +1
        self.position = (self.position + 1) % 60


    def atuadorAlertaVermelho(self, id):
        ultima_posicao = 0
        for i in range(60):
            if (self.valores[i][0] != 0):
                if (self.valores[i][1] == id):
                    if (self.valores[i][2] >= ultima_posicao):
                        ultima_posicao = i
                    
                
        aux = 0
        alerta_vermelho = 0
        contador = ultima_posicao % 60
        for i in range(60):
            if (self.valores[contador][1] == id and self.valores[contador][0] != 0 and aux <= 4):
                if (self.valores[contador][0] > 35):
                    alerta_vermelho += 1
                aux += 1
            contador -= 1
            contador = contador % 60
        if alerta_vermelho < 5:
            print("ERRO: Alerta Vermelho errado de Atuador(" + str(id) + ") pois nao teve as 5 ultimas leituras acima de 35")

    def atuadorAlertaAmarelo(self, id):
        ultima_posicao = 0
        for i in range(60):
            if (self.valores[i][0] != 0):
                if (self.valores[i][1] == id):
                    if (self.valores[i][2] >= ultima_posicao):
                        ultima_posicao = i
                    
                
        aux = 0
        alerta_amarelo = 0
        contador = ultima_posicao % 60
        for i in range(60):
            if (self.valores[contador][1] == id and self.valores[contador][0] != 0 and aux <= 14):
                if (self.valores[contador][0] > 35):
                    alerta_amarelo += 1
                aux += 1
            contador -= 1
            contador = contador % 60
        if alerta_amarelo < 5:
            print("ERRO: Alerta Amarelo errado de Atuador(" + str(id) + ") pois nao teve as 15 ultimas leituras acima de 35")

    def atuadorAlertaNormal(self, id):
        ultima_posicao = 0
        for i in range(60):
            if (self.valores[i][0] != 0):
                if (self.valores[i][1] == id):
                    if (self.valores[i][2] >= ultima_posicao):
                        ultima_posicao = i
                    
                
        aux = 0
        alerta_normal = 0
        contador = ultima_posicao % 60
        for i in range(60):
            if (self.valores[contador][1] == id and self.valores[contador][0] != 0 and aux <= 14):
                if (self.valores[contador][0] > 35):
                    alerta_normal += 1
                aux += 1
            contador -= 1
            contador = contador % 60
        if alerta_normal >= 5:
            print("ERRO: Alerta Normal errado de Atuador(" + str(id) + ") pois teve 5 dentre as 15 ultimas leituras acima de 35")

    def atuadorMedia(self, id, media):
        ultima_posicao = 0
        for i in range(60):
            if (self.valores[i][0] != 0):
                if (self.valores[i][1] == id):
                    if (self.valores[i][2] >= ultima_posicao):
                        ultima_posicao = i
                    
                
        aux = 0
        media_checagem = 0
        contador = ultima_posicao % 60
        for i in range(60):
        	if (self.valores[contador][1] == id and self.valores[contador][0] != 0):
        	    media_checagem += self.valores[contador][0]
        	    aux += 1
        	contador -= 1
        	contador = contador % 60
        if (aux > 0): media_checagem = media_checagem / aux
			
        if (media_checagem == 0):
        	if media != 0.0:
        	    print("ERRO: A media de " + str(id) + " foi " + str(media) + " mas seria 0!")
        elif abs( media_checagem - media ) > 0.001:            # checamos com determinado erro de precisão do float
            print("ERRO: A media de " + str(id) + " foi " + str(media) + " mas seria " + str(media_checagem) + "!")