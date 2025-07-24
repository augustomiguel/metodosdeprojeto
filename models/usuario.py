class Usuario:
    def __init__(self, nome, email):
        self.nome = nome
        self.email = email

    def exibir_dados(self):
        print(f"Nome: {self.nome}, Email: {self.email}")
