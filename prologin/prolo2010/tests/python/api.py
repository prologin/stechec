# -*- coding: iso-8859-1 -*-
from _api import *

# Taille de d�part du terrain.
TAILLE_DEPART = 19

# Le nombre maximal d'unit�s pouvant appartenir � une �quipe.
NBR_MAX_UNITES = 10

# Le temps, en nombre de tours, entre deux r�tr�cissement du terrain.
TEMPS_RETRECISSEMENT = 5

# Le nombre maximum en jeu de chaque carte.
MAX_CARTES = 2

# �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action.
(
    OK, # <- aucune erreur n'est survenue
    POSITION_INVALIDE, # <- la position sp�cifi�e est invalide
    CASE_OCCUPEE, # <- la case sur laquelle vous tentez de vous d�placer est occup�e
    PAS_A_PORTEE, # <- la cible n'est pas � port�e
    UNITE_KO, # <- l'unit� que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO
    UNITE_DEBOUT, # <- l'unit� que vous essayez de relever est d�j� debout
    QUOTA_DEPASSE, # <- nombre maximal d'unites, de spawn ou de relevages par tour d�pass�
    PLUS_DE_PA, # <- cette unit� a r�alis� toutes ses actions
    DEJA_ATTAQUE, # <- cette unit� a d�j� attaqu�
    UNITE_INTERDITE, # <- cette unit� ne peut pas �tre amen�e en renfort
    RENFORT_IMPOSSIBLE, # <- une unit� est d�j� pr�sente sur le spawn
    PAS_A_MOI, # <- l'unit� cibl�e n'est pas � vous
    PLUS_DE_CARTES, # <- il n'y a plus de cartes du type sp�cifi� dans votre main
    PHASE_CARTES_TERMINEE, # <- vous ne pouvez plus poser de cartes car vous avez fait une action
) = range(14)


# Le type d'une unit� sur le terrain
(
    PERROQUET, # <- 1 PA et port�e de 1
    SINGE, # <- 2 PA et port�e de 3
    CHAT, # <- 5 PA et port�e de 1
    KANGOUROU, # <- 3 PA et port�e de 2 (explose !)
) = range(4)


try:
    from collections import namedtuple
except ImportError:
    from operator import itemgetter as _itemgetter
    from keyword import iskeyword as _iskeyword
    import sys as _sys

    def namedtuple(typename, field_names, verbose=False):
        """Returns a new subclass of tuple with named fields.

        >>> Point = namedtuple('Point', 'x y')
        >>> Point.__doc__                   # docstring for the new class
        'Point(x, y)'
        >>> p = Point(11, y=22)             # instantiate with positional args or keywords
        >>> p[0] + p[1]                     # indexable like a plain tuple
        33
        >>> x, y = p                        # unpack like a regular tuple
        >>> x, y
        (11, 22)
        >>> p.x + p.y                       # fields also accessable by name
        33
        >>> d = p._asdict()                 # convert to a dictionary
        >>> d['x']
        11
        >>> Point(**d)                      # convert from a dictionary
        Point(x=11, y=22)
        >>> p._replace(x=100)               # _replace() is like str.replace() but targets named fields
        Point(x=100, y=22)

        """

        # Parse and validate the field names.  Validation serves two purposes,
        # generating informative error messages and preventing template injection attacks.
        if isinstance(field_names, basestring):
            field_names = field_names.replace(',', ' ').split() # names separated by whitespace and/or commas
        field_names = tuple(map(str, field_names))
        for name in (typename,) + field_names:
            if not all(c.isalnum() or c=='_' for c in name):
                raise ValueError('Type names and field names can only contain alphanumeric characters and underscores: %r' % name)
            if _iskeyword(name):
                raise ValueError('Type names and field names cannot be a keyword: %r' % name)
            if name[0].isdigit():
                raise ValueError('Type names and field names cannot start with a number: %r' % name)
        seen_names = set()
        for name in field_names:
            if name.startswith('_'):
                raise ValueError('Field names cannot start with an underscore: %r' % name)
            if name in seen_names:
                raise ValueError('Encountered duplicate field name: %r' % name)
            seen_names.add(name)

        # Create and fill-in the class template
        numfields = len(field_names)
        argtxt = repr(field_names).replace("'", "")[1:-1]   # tuple repr without parens or quotes
        reprtxt = ', '.join('%s=%%r' % name for name in field_names)
        dicttxt = ', '.join('%r: t[%d]' % (name, pos) for pos, name in enumerate(field_names))
        template = '''class %(typename)s(tuple):
        '%(typename)s(%(argtxt)s)' \n
        __slots__ = () \n
        _fields = %(field_names)r \n
        def __new__(_cls, %(argtxt)s):
            return _tuple.__new__(_cls, (%(argtxt)s)) \n
        @classmethod
        def _make(cls, iterable, new=tuple.__new__, len=len):
            'Make a new %(typename)s object from a sequence or iterable'
            result = new(cls, iterable)
            if len(result) != %(numfields)d:
                raise TypeError('Expected %(numfields)d arguments, got %%d' %% len(result))
            return result \n
        def __repr__(self):
            return '%(typename)s(%(reprtxt)s)' %% self \n
        def _asdict(t):
            'Return a new dict which maps field names to their values'
            return {%(dicttxt)s} \n
        def _replace(_self, **kwds):
            'Return a new %(typename)s object replacing specified fields with new values'
            result = _self._make(map(kwds.pop, %(field_names)r, _self))
            if kwds:
                raise ValueError('Got unexpected field names: %%r' %% kwds.keys())
            return result \n
        def __getnewargs__(self):
            return tuple(self) \n\n''' % locals()
        for i, name in enumerate(field_names):
            template += '        %s = _property(_itemgetter(%d))\n' % (name, i)
        if verbose:
            print template

        # Execute the template string in a temporary namespace and
        # support tracing utilities by setting a value for frame.f_globals['__name__']
        namespace = dict(_itemgetter=_itemgetter, __name__='namedtuple_%s' % typename,
                         _property=property, _tuple=tuple)
        try:
            exec template in namespace
        except SyntaxError, e:
            raise SyntaxError(e.message + ':\n' + template)
        result = namespace[typename]

        # For pickling to work, the __module__ variable needs to be set to the frame
        # where the named tuple is created.  Bypass this step in enviroments where
        # sys._getframe is not defined (Jython for example).
        if hasattr(_sys, '_getframe'):
            result.__module__ = _sys._getframe(1).f_globals.get('__name__', '__main__')

        return result
# Repr�sente une position sur le terrain du jeu.
position = namedtuple("position",
    'x ' # <- coordonn�e X
    'y ' # <- coordonn�e Y
)


# Contient les informations sur la taille du terrain du jeu.
taille_terrain = namedtuple("taille_terrain",
    'taille ' # <- taille actuelle du terrain
    'min_coord ' # <- coordonn�e minimale en X ou en Y
    'max_coord ' # <- coordonn�e maximale en X ou en Y
)


# Donne les caract�ristiques d'un type d'unit�.
caracs = namedtuple("caracs",
    'pa_init ' # <- nombre de points d'actions par tour
    'portee ' # <- port�e maximale de l'unit�
)


# Repr�sente une unit� sur le terrain.
unite = namedtuple("unite",
    'pos ' # <- la position de l'unit�
    'ennemi ' # <- vrai si l'unit� appartient � l'ennemi
    'type_unite_actuel ' # <- le type de l'unit�, qui change si l'unit� est d�guis�e
    'vrai_type_unite ' # <- le vrai type de l'unit� (qui ne change pas lors du d�guisement)
    'ko ' # <- une valeur n�gative si l'unit� n'est pas KO, sinon le nombre de marqueurs KO sur l'unit�
    'pa ' # <- le nombre de PA restants � l'unit�
    'attaques ' # <- le nombre d'attaques restants � l'unit�
    'attaques_gratuites ' # <- le nombre d'attaques gratuites (voir la partie banzai)
    'id ' # <- l'identifiant unique de l'unit�
)


# Repr�sente l'ensemble des cartes que vous pouvez utiliser.
cartes = namedtuple("cartes",
    'potion ' # <- le nombre de cartes � Potion magique �
    'deguisement ' # <- le nombre de cartes � D�guisement �
    'banzai ' # <- le nombre de cartes � Banza� �
    'pacifisme ' # <- le nombre de cartes � Pacifisme �
)


