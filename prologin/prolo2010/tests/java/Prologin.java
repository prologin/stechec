// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_java.rb

public class Prologin extends Interface
{
  // Fonction appell�e au d�but de la partie.
  public void init_game()
  {
    // Place ton code ici
  }


  // Fonction appell�e pour la phase de retrait de KO.
  public Position retirer_ko()
  {
    // Place ton code ici
      return null;
  }

    private void toto(){
	throw new RuntimeException();
    }


  // Fonction appell�e pour la phase de jeu.
  public void jouer()
  {
      try{
	  toto();
      } catch (Exception e){
	  System.out.println("catched !");
      }
  }


  // Fonction appell�e � la fin de la partie.
  public void end_game()
  {
    // Place ton code ici
  }


}
