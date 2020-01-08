<?hh // strict

namespace Zynga\Framework\Environment\Command\Finder\V1;

use
  Zynga\Framework\Environment\Command\Finder\V1\Exceptions\CommandNotFoundException
;
use Zynga\Framework\Environment\CodePath\V1\CodePath;

class Finder {

  public static function getPaths(): Vector<string> {
    $paths = Vector {};
    $paths[] = '/usr/local/bin';
    $paths[] = '/usr/bin';
    $paths[] = '/bin';
    $paths[] = CodePath::getRoot().'/bin';
    $paths[] = CodePath::getRoot().'/vendor/bin';
    return $paths;
  }

  public static function find(string $command): string {
    return self::get($command);
  }

  public static function get(string $command): string {

    $paths = self::getPaths();

    foreach ($paths as $path) {
      $candidate = $path.'/'.$command;
      if (is_executable($candidate)) {
        return $candidate;
      }
    }

    throw new CommandNotFoundException('Unable to find command='.$command);

  }

}
