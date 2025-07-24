from models.usuario import Usuario
from views.user_view import UserView

class AdministradorDeUsuarios(Usuario):
    def __init__(self, nome, email):
        super().__init__(nome, email)
        self.view = UserView()

    def mostrar_interface_usuario(self):
        self.view.mostrar()
