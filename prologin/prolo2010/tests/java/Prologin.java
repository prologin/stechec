// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_java.rb

public class Prologin extends Interface
{
  // Fonction appellée au début de la partie.
  public void init_game()
  {
    // Place ton code ici
  }


  // Fonction appellée pour la phase de retrait de KO.
  public Position retirer_ko()
  {
    // Place ton code ici
      return null;
  }

    private void toto(){
	throw new RuntimeException();
    }


  // Fonction appellée pour la phase de jeu.
  public void jouer()
  {
      try{
	  toto();
      } catch (Exception e){
	  System.out.println("catched !");
      }
  }


  // Fonction appellée à la fin de la partie.
  public void end_game()
  {
    // Place ton code ici
  }


}
