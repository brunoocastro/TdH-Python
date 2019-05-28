import pygame
import random

num_rings = 5


def newLargura(self):

	largura = 170 + self.peso * 20
	return largura

def newCor(self):

	cor = self.peso * 200/5
	return cor

def posInitY(self):

	posy = coluna1.posx
	return posy

def newColor():
    colors = ["blue","black","brown","red","yellow","green","orange","beige","turquoise","pink"]
    random.shuffle(colors)
    return colors[0]

def createDisco(num_rings):
	for ring in range(num_rings):
		print 'Disco',ring
		ring = Disco(ring)
		ring.specs()


class Disco:
	"""Classe responsavel pelas caracteristicas dos discos"""
	def __init__(self, peso):

		self.peso = peso
		# self.nome = "disco"+self.peso
		self.cor = newColor()
		self.altura = 40
		self.largura = newLargura(self)
		self.posx = coluna1.posx
		# self.posy = coluna1.posy.(self.peso)

	def specs(self):
		# print "Nome:", self.nome
		print "Peso:", self.peso
		print "Cor:", self.cor
		print "Altura:", self.altura
		print "Largura:", self.largura
		print "\n \n"

class Coluna:
	"""Classe responsavel pelas caracteriscas das Coluna"""
 	def __init__(self, peso, num_discos):

		self.posx = 200
		self.posy = 200
		
coluna1 = Coluna(1, num_rings)
coluna2 = Coluna(2, num_rings)
coluna3 = Coluna(3, num_rings)
createDisco(num_rings)
#disco1 = Disco(1)

#disco1.specs()



