class SA:
    def __init__(self, quantsensores):
        """ Temos a posição do vetor, um vetor de 60 posições com 3 posições inteiras e
            a quantidade de leituras feita por cada Sensor. """
        self.position = 0                          # Posição atual de escrita do vetor
        self.valores = [[0]*3]*60                  # objeto Valor [valor,id_sensor,id_leitura]
        self.idLeitura = [0]*quantsensores         # guarda a quantidade de medições feitas por cada sensor

    def sensor(self, id, temperatura):
        """ Adiciona o valor medido pelo sensor na simulação que temos do Vetor """
        self.valores[self.position] = [temperatura, id, self.idLeitura[id]]
        self.idLeitura[id] = self.idLeitura[id] +1
        self.position = (self.position + 1) % 60


    def atuadorAlertaVermelho(self, id):
        """ Simula o comportamento adotado pelos Atuadores no programa em Java e verifica se o alerta vermelho omitido está correto """
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
        """ Simula o comportamento adotado pelos Atuadores no programa em Java e verifica se o alerta amarelo omitido está correto """
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
        """ Simula o comportamento adotado pelos Atuadores no programa em Java e verifica se o alerta normal omitido está correto """
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
        """ Calcula a Média dos valores de cada Sensor e verifica se a média lida no log condiz com a média calculada na simulação. 
            Para isso adotamos um erro do valor entre as médias de 0.001 """
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