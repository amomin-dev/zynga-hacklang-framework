<?hh // strict

namespace Zynga\Framework\Service\V2\Client\Mock;

use Zynga\Framework\Service\V2\Client\Batch as BaseBatch;
use Zynga\Framework\Service\V2\Interfaces\ResponseInterface;
use Zynga\Framework\Service\V2\Response\Mock\Base as MockResponse;

class Batch extends BaseBatch {

  public function hasResponse(int $requestId): bool {
    return false;
  }

  public function requestSent(int $requestId): bool {
    return false;
  }

  public function getResponse(int $requestId): ResponseInterface {
    return new MockResponse(true, 200);
  }

  public function isBatchStarted(): bool {
    return false;
  }
}
